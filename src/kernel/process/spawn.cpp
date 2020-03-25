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

#include "msos/kernel/process/process.hpp"
#include "msos/kernel/process/context_switch.hpp"
#include "msos/kernel/process/scheduler.hpp"
#include "msos/fs/mount_points.hpp"
#include <msos/dynamic_linker/dynamic_linker.hpp>
#include <msos/dynamic_linker/environment.hpp>

#include "msos/syscalls/syscalls.hpp"

#include <hal/interrupt/systick.hpp>

msos::kernel::process::ProcessManager processes;

constexpr std::size_t default_stack_size = 1500;
msos::dl::DynamicLinker dynamic_linker;

uint32_t get_lot_at(uint32_t address)
{
    return dynamic_linker.get_lot_for_module_at(address);
}

struct ExecInfo
{
    const char* path;
    SymbolEntry* entries;
    int number_of_entries;
};

static msos::dl::Environment<8> env{
        msos::dl::SymbolAddress{"strlen", &strlen},
        msos::dl::SymbolAddress{"memcpy", &memcpy},
        msos::dl::SymbolAddress{"memcmp", &memcmp},
        msos::dl::SymbolAddress{"memset", &memset},
        msos::dl::SymbolAddress{"strstr", &strstr},
        msos::dl::SymbolAddress{"write", &write},
        msos::dl::SymbolAddress{"scanf", reinterpret_cast<uint32_t*>(&scanf)},
        msos::dl::SymbolAddress{"printf", reinterpret_cast<uint32_t*>(&printf)}
};

pid_t spawn(void (*start_routine) (void *), void *arg)
{
    auto& child = processes.create_process(
        reinterpret_cast<std::size_t>(start_routine), default_stack_size, reinterpret_cast<std::uint32_t>(arg));
    return child.pid();
}

pid_t spawn_root_process(void (*start_routine) (void *), void *arg)
{
    hal::interrupt::disable_systick();
    msos::kernel::process::Scheduler::get().set_process_manager(processes);
    processes.create_process(reinterpret_cast<std::size_t>(start_routine), default_stack_size);
    msos::kernel::process::Scheduler::get().schedule_next();

    msos::process::initialize_context_switching();
    trigger_syscall(SyscallNumber::SYSCALL_START_ROOT_PROCESS, NULL, NULL);
    while(1) {}
}

int exec_process(ExecInfo* info)
{
    printf("Executing process: %s, with env: %p, size: %d\n", info->path, info->entries, info->number_of_entries);
    auto& mount_points = msos::fs::mount_points.get_mounted_points();

    msos::fs::IFileSystem* fs;
    std::string_view best_mount_point;
    std::string_view path_to_executable(info->path);
    for (auto& point : mount_points)
    {
        std::size_t index = path_to_executable.find(point.point);
        if (index != std::string_view::npos)
        {
            if (point.point.size() > best_mount_point.size())
            {
                best_mount_point = point.point;
            }
        }
    }

    if (best_mount_point != "/")
    {
        path_to_executable.remove_prefix(best_mount_point.size());
    }

    fs = msos::fs::mount_points.get_mounted_filesystem(best_mount_point);

    if (fs == nullptr)
    {
        printf("Fs not exists\n");
        return -1;
    }

    std::unique_ptr<msos::fs::IFile> file = fs->get(path_to_executable);

    if (!file)
    {
        printf("File not exists\n");
        return -1;
    }

    std::size_t module_address = reinterpret_cast<uint32_t>(file->data().data());

    const msos::dl::LoadedModule* module;
    if (info->entries)
    {
        module = dynamic_linker.load_module(module_address, msos::dl::LoadingModeCopyData, info->entries, info->number_of_entries);
        delete info;
    }
    else
    {
        module = dynamic_linker.load_module(module_address, msos::dl::LoadingModeCopyData, env);
        delete info;
    }
    if (module)
    {
        printf("Execute1\n");
        return module->execute();
    }
    return -1;
}

static bool is_first = true;

void exec(const char* path, void *arg, SymbolEntry* entries, int number_of_entries)
{
    if (is_first)
    {
        uint32_t address_of_lot_getter = reinterpret_cast<uint32_t>(&get_lot_at);
        uint32_t* lot_in_memory = reinterpret_cast<uint32_t*>(0x20000000);
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

pid_t spawn_exec(const char* path, void *arg, SymbolEntry* entries, int number_of_entries)
{
    if (is_first)
    {
        uint32_t address_of_lot_getter = reinterpret_cast<uint32_t>(&get_lot_at);
        uint32_t* lot_in_memory = reinterpret_cast<uint32_t*>(0x20000000);
        *lot_in_memory = address_of_lot_getter;
        is_first = false;
    }

    ExecInfo* info = new ExecInfo{
        .path = path,
        .entries = entries,
        .number_of_entries = number_of_entries
    };

    auto& child = processes.create_process(
        reinterpret_cast<std::size_t>(exec_process), default_stack_size, reinterpret_cast<uint32_t>(info));
    return child.pid();
}
