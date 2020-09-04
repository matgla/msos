// This file is part of MSOS project.
// Copyright (C) 2020 Mateusz Stadnik
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

#include "msos/kernel/process/spawn.hpp"

#include "symbol_codes.h"
#include "msos/posix/dirent.h"
#include "msos/posix/unistd_impl.hpp"
#include "arch/armv7-m/arm_process.hpp"
#include "msos/kernel/process/process.hpp"
#include "msos/kernel/process/process_manager.hpp"
#include "msos/kernel/process/context_switch.hpp"
#include "msos/kernel/process/scheduler.hpp"
#include "msos/fs/mount_points.hpp"
#include "msos/fs/vfs.hpp"
#include <msos/dynamic_linker/dynamic_linker.hpp>
#include <msos/dynamic_linker/environment.hpp>
#include <eul/error/error_code.hpp>
#include <eul/filesystem/path.hpp>
#include <eul/utils/unused.hpp>
#include <memory>

#include "msos/libc/printf.hpp"

#include "msos/syscalls/syscalls.hpp"

#include <hal/interrupt/systick.hpp>

#include "msos/usart_printer.hpp"


constexpr std::size_t default_stack_size = 728;
static msos::dl::DynamicLinker dynamic_linker;

static UsartWriter writer;

std::size_t get_lot_at(uint32_t address)
{
    return dynamic_linker.get_lot_for_module_at(address);
}

struct ExecInfo
{
    const char* path;
    const SymbolEntry* entries;
    int number_of_entries;
    int argc;
    char** argv;
};

extern "C"
{
    void* memchr_wrapper(const void* s, int c, size_t n)
    {
        return const_cast<void*>(memchr(s, c, n));
    }
}

static msos::dl::Environment<16> env{
    msos::dl::SymbolAddress{SymbolCode::libc_strlen, &strlen},
    msos::dl::SymbolAddress{SymbolCode::libc_scanf, reinterpret_cast<uint32_t*>(&_scanf)},
    msos::dl::SymbolAddress{SymbolCode::libc_printf, reinterpret_cast<uint32_t*>(&_printf)},
    msos::dl::SymbolAddress{SymbolCode::libc_getc, &getc},
    msos::dl::SymbolAddress{SymbolCode::libc_memset, &memset},
    msos::dl::SymbolAddress{SymbolCode::libc_memcpy, &memcpy},
    msos::dl::SymbolAddress{SymbolCode::libc_fopen, &fopen},
    msos::dl::SymbolAddress{SymbolCode::posix__readdir, &readdir},
    msos::dl::SymbolAddress{SymbolCode::posix__opendir, &opendir},
    msos::dl::SymbolAddress{SymbolCode::posix__closedir, &closedir},
    msos::dl::SymbolAddress{SymbolCode::posix_getcwd, &getcwd},
    msos::dl::SymbolAddress{SymbolCode::posix_chdir, &chdir},
    msos::dl::SymbolAddress{SymbolCode::posix_getopt, &getopt},
    msos::dl::SymbolAddress{SymbolCode::posix_optind, &optind},
    msos::dl::SymbolAddress{SymbolCode::posix_optarg, &optarg},
    msos::dl::SymbolAddress{SymbolCode::posix_optopt, &optopt},
};

pid_t spawn(void (*start_routine) (void *), void *arg)
{
    auto& child = msos::kernel::process::ProcessManager<msos::arch::armv7m::ArmProcess>::get().create_process(
        reinterpret_cast<std::size_t>(start_routine), default_stack_size, reinterpret_cast<std::size_t>(arg));
    return child.pid();
}

pid_t spawn_root_process(void (*start_routine) (void *), void *arg, std::size_t stack_size)
{
    UNUSED1(arg);
    hal::interrupt::disable_systick();
    msos::kernel::process::ProcessManager<msos::arch::armv7m::ArmProcess>::get().create_process(reinterpret_cast<std::size_t>(start_routine), stack_size);
    auto* scheduler = msos::kernel::process::Scheduler::get();
    if (scheduler)
    {
        scheduler->schedule_next();
    }

    msos::process::initialize_context_switching();
    trigger_syscall(SyscallNumber::SYSCALL_START_ROOT_PROCESS, NULL, NULL);
    while(1) {}
}

int exec_process(ExecInfo* info)
{
    std::string_view path_to_executable(info->path);
    eul::filesystem::path path = eul::filesystem::path(info->path).lexically_normal();

    msos::fs::Vfs& root_fs = msos::fs::Vfs::instance();

    msos::fs::IFileSystem* dest_fs = root_fs.get_child_fs(path.c_str());

    std::unique_ptr<msos::fs::IFile> file = root_fs.get(path.c_str(), 0);

    if (!file)
    {
        return -2;
    }

    if (dest_fs->name() == "AppFs")
    {
        if (file->data() != nullptr)
        {
            int(*fun)() = reinterpret_cast<int(*)()>(file->data());
            return fun();
        }
    }

    const std::size_t* module_address = reinterpret_cast<const std::size_t*>(file->data());

    const msos::dl::LoadedModule* module;
    eul::error::error_code ec;

    if (info->entries)
    {
        module = dynamic_linker.load_module(module_address, msos::dl::LoadingModeCopyData, info->entries, info->number_of_entries, ec);
        delete info;
    }
    else
    {
        module = dynamic_linker.load_module(module_address, msos::dl::LoadingModeCopyData, env, ec);
        if (ec)
        {
            writer << ec.message() << endl;
        }
        delete info;
    }
    if (module)
    {
        return module->execute(info->argc, info->argv);
    }
    else
    {
        if (file->data() != nullptr)
        {
            int(*fun)() = reinterpret_cast<int(*)()>(file->data());
            return fun();
        }
    }
    // TODO: change to error code as argument (application returns also)
    return -2;
}

static bool is_first = true;

int exec(const char* path, int argc, char* argv[], const SymbolEntry* entries, int number_of_entries)
{
    if (is_first)
    {
        std::size_t address_of_lot_getter = reinterpret_cast<std::size_t>(&get_lot_at);
        std::size_t* lot_in_memory = reinterpret_cast<std::size_t*>(0x20000000);
        *lot_in_memory = address_of_lot_getter;
        is_first = false;
    }

    ExecInfo* info = new ExecInfo{
        .path = path,
        .entries = entries,
        .number_of_entries = number_of_entries,
        .argc = argc,
        .argv = argv
    };

    optind = 0;
    optopt = '?';
    opterr = 1;
    return exec_process(info);
}

pid_t spawn_exec(const char* path, void *arg, const SymbolEntry* entries, int number_of_entries, std::size_t stack_size)
{
    UNUSED1(arg);
    if (is_first)
    {
        std::size_t address_of_lot_getter = reinterpret_cast<std::size_t>(&get_lot_at);
        std::size_t* lot_in_memory = reinterpret_cast<std::size_t*>(0x20000000);
        *lot_in_memory = address_of_lot_getter;
        is_first = false;
    }

    ExecInfo* info = new ExecInfo{
        .path = path,
        .entries = entries,
        .number_of_entries = number_of_entries
    };

    auto& child = msos::kernel::process::ProcessManager<msos::arch::armv7m::ArmProcess>::get().create_process(
        reinterpret_cast<std::size_t>(exec_process), stack_size, reinterpret_cast<std::size_t>(info));
    return child.pid();
}
