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
#include <msos/dynamic_linker/dynamic_linker.hpp>
#include <msos/dynamic_linker/environment.hpp>
#include <eul/error/error_code.hpp>

#include "msos/libc/printf.hpp"

#include "msos/syscalls/syscalls.hpp"

#include <hal/interrupt/systick.hpp>

#include "msos/usart_printer.hpp"

msos::kernel::process::ProcessManager processes;

constexpr std::size_t default_stack_size = 728;
msos::dl::DynamicLinker dynamic_linker;

static UsartWriter writer;

uint32_t get_lot_at(uint32_t address)
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

static msos::dl::Environment<16> env{
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
        msos::dl::SymbolAddress{"opendir", reinterpret_cast<uint32_t*>(&opendir)},
        msos::dl::SymbolAddress{"readdir", reinterpret_cast<uint32_t*>(&readdir)},
        msos::dl::SymbolAddress{"closedir", reinterpret_cast<uint32_t*>(&closedir)},
        msos::dl::SymbolAddress{"memchr", reinterpret_cast<uint32_t*>(&memchr_wrapper)},
        msos::dl::SymbolAddress{"_ZSt24__throw_out_of_range_fmtPKcz", reinterpret_cast<uint32_t*>(&std::__throw_out_of_range_fmt)},
        msos::dl::SymbolAddress{"isspace", reinterpret_cast<uint32_t*>(&isspace)},


};

pid_t spawn(void (*start_routine) (void *), void *arg)
{
    auto& child = processes.create_process(
        reinterpret_cast<std::size_t>(start_routine), default_stack_size, reinterpret_cast<std::size_t>(arg));
    return child.pid();
}

pid_t spawn_root_process(void (*start_routine) (void *), void *arg, std::size_t stack_size)
{
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
    std::string_view path_to_executable(info->path);

    writer << "Trying to execute process: " << info->path << endl;
    msos::fs::IFileSystem* root_fs = msos::fs::mount_points.get_mounted_filesystem("/");

    if (root_fs == nullptr)
    {
        writer << "Root fs not found";

        return -1;
    }

    std::unique_ptr<msos::fs::IFile> file = root_fs->get(path_to_executable);

    if (!file)
    {
        return -1;
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
    return -1;
}

static bool is_first = true;

void exec(const char* path, void *arg, const SymbolEntry* entries, int number_of_entries)
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
        .number_of_entries = number_of_entries
    };

    exec_process(info);
}

pid_t spawn_exec(const char* path, void *arg, const SymbolEntry* entries, int number_of_entries, std::size_t stack_size)
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
        .number_of_entries = number_of_entries
    };

    auto& child = processes.create_process(
        reinterpret_cast<std::size_t>(exec_process), stack_size, reinterpret_cast<std::size_t>(info));
    return child.pid();
}
