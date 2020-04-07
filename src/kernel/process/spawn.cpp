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

#include "msos/posix/dirent.h"
#include "msos/kernel/process/process.hpp"
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

msos::kernel::process::ProcessManager processes;

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
};

extern "C"
{
    void* memchr_wrapper(const void* s, int c, size_t n)
    {
        return const_cast<void*>(memchr(s, c, n));
    }
}

static msos::dl::Environment<10> env{
        msos::dl::SymbolAddress{"strlen", &strlen},
        msos::dl::SymbolAddress{"memcpy", &memcpy},
        msos::dl::SymbolAddress{"memcmp", &memcmp},
        msos::dl::SymbolAddress{"memset", &memset},
        msos::dl::SymbolAddress{"strstr", &strstr},
        msos::dl::SymbolAddress{"write", &write},
        msos::dl::SymbolAddress{"scanf", reinterpret_cast<uint32_t*>(&_scanf)},
        msos::dl::SymbolAddress{"printf", reinterpret_cast<uint32_t*>(&_printf)},
        msos::dl::SymbolAddress{"spawn_exec", reinterpret_cast<uint32_t*>(&spawn_exec)},
        msos::dl::SymbolAddress{"exec", reinterpret_cast<uint32_t*>(&exec)},
        // msos::dl::SymbolAddress{"opendir", reinterpret_cast<uint32_t*>(&opendir)},
        // msos::dl::SymbolAddress{"readdir", reinterpret_cast<uint32_t*>(&readdir)},
        // msos::dl::SymbolAddress{"closedir", reinterpret_cast<uint32_t*>(&closedir)},
        // msos::dl::SymbolAddress{"memchr", reinterpret_cast<uint32_t*>(&memchr_wrapper)},
        // msos::dl::SymbolAddress{"_ZSt24__throw_out_of_range_fmtPKcz", reinterpret_cast<uint32_t*>(&std::__throw_out_of_range_fmt)},
        // msos::dl::SymbolAddress{"isspace", reinterpret_cast<uint32_t*>(&isspace)},


};

pid_t spawn(void (*start_routine) (void *), void *arg)
{
    auto& child = processes.create_process(
        reinterpret_cast<std::size_t>(start_routine), default_stack_size, reinterpret_cast<std::size_t>(arg));
    return child.pid();
}

pid_t spawn_root_process(void (*start_routine) (void *), void *arg, std::size_t stack_size)
{
    UNUSED1(arg);
    hal::interrupt::disable_systick();
    msos::kernel::process::Scheduler::get().set_process_manager(processes);
    processes.create_process(reinterpret_cast<std::size_t>(start_routine), stack_size);
    msos::kernel::process::Scheduler::get().schedule_next();

    msos::process::initialize_context_switching();
    trigger_syscall(SyscallNumber::SYSCALL_START_ROOT_PROCESS, NULL, NULL);
    while(1) {}
}

int exec_process(ExecInfo* info)
{
    writer << "Trying to execute process: " << info->path << endl;
    std::string_view path_to_executable(info->path);
    eul::filesystem::path path = eul::filesystem::path(info->path).lexically_normal();

    msos::fs::Vfs& root_fs = msos::fs::Vfs::instance();

    writer << "Go further" << endl;
    msos::fs::IFileSystem* dest_fs = root_fs.get_child_fs(path.c_str());
    writer << "destfs: " << dest_fs->name() << endl;

    std::unique_ptr<msos::fs::IFile> file = root_fs.get(path.c_str());

    if (!file)
    {
        return -1;
    }
    writer << "File exists" << endl;
    writer << "File name: " << file->name() << endl;
    if (file->name().rfind(".bin") == std::string_view::npos)
    {
        writer << "Bin not exists" << endl;
        return -1;
    }

    if (dest_fs->name() == "AppFs")
    {
        writer << "AppFs is my fs" << endl;
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
        delete info;
    }
    if (module)
    {
        return module->execute();
    }
    else
    {
        if (file->data() != nullptr)
        {
            int(*fun)() = reinterpret_cast<int(*)()>(file->data());
            return fun();
        }
    }
    return -1;
}

static bool is_first = true;

void exec(const char* path, void *arg, const SymbolEntry* entries, int number_of_entries)
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

    exec_process(info);
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

    auto& child = processes.create_process(
        reinterpret_cast<std::size_t>(exec_process), stack_size, reinterpret_cast<std::size_t>(info));
    return child.pid();
}
