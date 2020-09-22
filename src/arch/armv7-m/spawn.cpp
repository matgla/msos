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
#include "curses_symbol_codes.h"
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
#include <curses.h>

#include "msos/libc/printf.hpp"

#include "msos/syscalls/syscalls.hpp"

#include <hal/interrupt/systick.hpp>

#include "msos/usart_printer.hpp"

extern "C"
{
extern uint32_t _kernel_ram_start;
} // extern "C"


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

void delete_port(void* ptr)
{
    ::operator delete(ptr);
}

void init_path(eul::filesystem::path* self, const char* path)
{
    new (self) eul::filesystem::path(path);
}

template <typename, class...>
class address_resolver;


template<typename T, class ReturnType, class... Args>
class address_resolver<T, ReturnType(Args...)>
{
public:
    constexpr static void* get(ReturnType(T::*member)(Args...))
    {
        #pragma GCC diagnostic push
        #pragma GCC diagnostic ignored "-Wpmf-conversions"
        return reinterpret_cast<void*>(member);
        #pragma GCC diagnostic pop
    }

    constexpr static void* get(ReturnType(T::*member)(Args...) const)
    {
        #pragma GCC diagnostic push
        #pragma GCC diagnostic ignored "-Wpmf-conversions"
        return reinterpret_cast<void*>(member);
        #pragma GCC diagnostic pop
    }
};

template <class ReturnType, class... Args>
class address_resolver<ReturnType(Args...)>
{
public:
    constexpr static void* get(ReturnType(*member)(Args...))
    {
        #pragma GCC diagnostic push
        #pragma GCC diagnostic ignored "-Wpmf-conversions"
        return reinterpret_cast<void*>(member);
        #pragma GCC diagnostic pop
    }

    constexpr static void* get(ReturnType(*member)(Args..., ...))
    {
        #pragma GCC diagnostic push
        #pragma GCC diagnostic ignored "-Wpmf-conversions"
        return reinterpret_cast<void*>(member);
        #pragma GCC diagnostic pop
    }
};


using namespace eul::filesystem;

static msos::dl::Environment env{
    msos::dl::SymbolAddress{SymbolCode::libc_strlen, &strlen},
    msos::dl::SymbolAddress{SymbolCode::libc_scanf, reinterpret_cast<uint32_t*>(&_scanf)},
    msos::dl::SymbolAddress{SymbolCode::libc_printf, reinterpret_cast<uint32_t*>(&_printf)},
    msos::dl::SymbolAddress{SymbolCode::libc_getc, &getc},
    msos::dl::SymbolAddress{SymbolCode::libc_memset, &memset},
    msos::dl::SymbolAddress{SymbolCode::libc_memcpy, &memcpy},
    msos::dl::SymbolAddress{SymbolCode::libc_fopen, &fopen},
    msos::dl::SymbolAddress{SymbolCode::libc_fclose, &fclose},
    msos::dl::SymbolAddress{SymbolCode::libc_fgets, &fgets},
    msos::dl::SymbolAddress{SymbolCode::libc_fflush, &fflush},
    msos::dl::SymbolAddress{SymbolCode::libc_fprintf, &fprintf},
    msos::dl::SymbolAddress{SymbolCode::libc_fread, &fread},
    msos::dl::SymbolAddress{SymbolCode::libc_fseek, &fseek},
    msos::dl::SymbolAddress{SymbolCode::libc_ftell, &ftell},
    msos::dl::SymbolAddress{SymbolCode::libc_fwrite, &fwrite},
    msos::dl::SymbolAddress{SymbolCode::libc_rand, &rand},
    msos::dl::SymbolAddress{SymbolCode::libc_localtime, &localtime},
    msos::dl::SymbolAddress{SymbolCode::libc_time, &time},
    msos::dl::SymbolAddress{SymbolCode::libc__ctype_, &_ctype_},
    msos::dl::SymbolAddress{SymbolCode::libc_exit, &exit},
    msos::dl::SymbolAddress{SymbolCode::libc__impure_ptr, &_impure_ptr},
    msos::dl::SymbolAddress{SymbolCode::libc_system, &system},
    msos::dl::SymbolAddress{SymbolCode::libc_putchar, &putchar},
    msos::dl::SymbolAddress{SymbolCode::libc_strcmp, &strcmp},
    msos::dl::SymbolAddress{SymbolCode::posix_readdir, &readdir},
    msos::dl::SymbolAddress{SymbolCode::posix_opendir, &opendir},
    msos::dl::SymbolAddress{SymbolCode::posix_closedir, &closedir},
    msos::dl::SymbolAddress{SymbolCode::posix_getcwd, &getcwd},
    msos::dl::SymbolAddress{SymbolCode::posix_chdir, &chdir},
    msos::dl::SymbolAddress{SymbolCode::posix_getopt, &getopt},
    msos::dl::SymbolAddress{SymbolCode::posix_optind, &optind},
    msos::dl::SymbolAddress{SymbolCode::posix_optarg, &optarg},
    msos::dl::SymbolAddress{SymbolCode::posix_optopt, &optopt},
    msos::dl::SymbolAddress{SymbolCode::posix_sleep, &sleep},
    msos::dl::SymbolAddress{SymbolCode::libstdcpp__ZdlPvj, &delete_port},
    msos::dl::SymbolAddress{SymbolCode::eul__ZN3eul10filesystem4pathC1EPKc, &init_path},
    msos::dl::SymbolAddress{SymbolCode::eul__ZNK3eul10filesystem4path5c_strEv, address_resolver<path, const char*()>::get(&path::c_str)},
    msos::dl::SymbolAddress{SymbolCode::eul__ZN3eul10filesystem4pathpLERKSt17basic_string_viewIcSt11char_traitsIcEE, address_resolver<path, path&(const std::string_view&)>::get(&path::operator+=)},
    msos::dl::SymbolAddress{SymbolCode::eul__ZN3eul10filesystem4pathpLERKS1_, address_resolver<path, path&(const path&)>::get(&path::operator+=)},
    msos::dl::SymbolAddress{SymbolCode::eul__ZNK3eul10filesystem4path16lexically_normalEv, address_resolver<path, path()>::get(&path::lexically_normal)},
    msos::dl::SymbolAddress{SymbolCode::eul__ZNK3eul10filesystem4path11is_absoluteEv, address_resolver<path, bool()>::get(&path::is_absolute)},
    // add_curses_symbol_codes()
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
    eul::filesystem::path path(info->path);
    path = path.lexically_normal();

    msos::fs::Vfs& root_fs = msos::fs::Vfs::instance();

    msos::fs::IFileSystem* dest_fs = root_fs.get_child_fs(path.c_str());

    std::unique_ptr<msos::fs::IFile> file = root_fs.get(path.c_str(), 0);


    if (!file)
    {
        printf ("File not found\n");
        errno = ENOENT;
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
        module = dynamic_linker.load_module(module_address, msos::dl::LoadingModeCopyText, info->entries, info->number_of_entries, ec);
        writer << "Module loaded" << endl;
    }
    else
    {
        module = dynamic_linker.load_module(module_address, msos::dl::LoadingModeCopyText, env, ec);
        if (ec)
        {
            writer << ec.message() << endl;
        }
    }
    if (module)
    {
        writer << "Module exists: " << hex << module->start_address_ << endl;

        int rc = module->execute(info->argc, info->argv);
        dynamic_linker.unload_module(module);
        return rc;
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
    return -1;
}

static bool is_first = true;

int exec(const char* path, int argc, char* argv[], const SymbolEntry* entries, int number_of_entries)
{
    printf("I am here\n");
    if (is_first)
    {
        std::size_t address_of_lot_getter = reinterpret_cast<std::size_t>(&get_lot_at);
        std::size_t* lot_in_memory = reinterpret_cast<std::size_t*>(&_kernel_ram_start);
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
    optarg = nullptr;
    printf("Execute process\n");
    int rc = exec_process(info);
    delete info;
    return rc;
}

pid_t spawn_exec(const char* path, void *arg, const SymbolEntry* entries, int number_of_entries, std::size_t stack_size)
{
    UNUSED1(arg);
    if (is_first)
    {
        std::size_t address_of_lot_getter = reinterpret_cast<std::size_t>(&get_lot_at);
        std::size_t* lot_in_memory = reinterpret_cast<std::size_t*>(&_kernel_ram_start);
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
