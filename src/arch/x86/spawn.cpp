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

#include <iostream>

msos::kernel::process::ProcessManager processes;

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

pid_t spawn(void (*start_routine) (void *), void *arg)
{
    return 0;
}

pid_t spawn_root_process(void (*start_routine) (void *), void *arg, std::size_t stack_size)
{
    UNUSED1(arg);
    // msos::kernel::process::Scheduler::get().set_process_manager(processes);
    processes.create_process(reinterpret_cast<std::size_t>(start_routine), stack_size);
    // msos::kernel::process::Scheduler::get().schedule_next();

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

    std::unique_ptr<msos::fs::IFile> file = root_fs.get(path.c_str());

    if (!file)
    {
        std::cerr << "File not found" << std::endl;
        return -1;
    }
    if (file->name().rfind(".bin") == std::string_view::npos)
    {
        std::cerr << "Bin not exists" << std::endl;
        return -1;
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
        std::cerr << "Loading module " << std::endl;
        // module = dynamic_linker.load_module(module_address, msos::dl::LoadingModeCopyData, info->entries, info->number_of_entries, ec);
        delete info;
    }
    else
    {
        std::cerr << "Loading module 2" << std::endl;

        // module = dynamic_linker.load_module(module_address, msos::dl::LoadingModeCopyData, env, ec);
        delete info;
    }
    if (module)
    {
        std::cerr << "Moduel execute" << std::endl;
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

void exec(const char* path, void *arg, const SymbolEntry* entries, int number_of_entries)
{
    ExecInfo* info = new ExecInfo{
        .path = path,
        .entries = entries,
        .number_of_entries = number_of_entries
    };

    exec_process(info);
}

pid_t spawn_exec(const char* path, void *arg, const SymbolEntry* entries, int number_of_entries, std::size_t stack_size)
{
    return 0;
}
