// This file is part of MSOS project. 
// Copyright (C) 2019 Mateusz Stadnik
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

#include "msos/kernel/process/scheduler.hpp"

#include "msos/kernel/process/process_manager.hpp"

#include <cstdio>

namespace msos
{
namespace kernel
{
namespace process
{

Scheduler::Scheduler(ProcessManager& processes)
    : processes_(processes)
    , current_process_(processes_.get_processes().begin())
{
} 

const Process& Scheduler::current_process() const 
{
    return *current_process_; 
}

Process& Scheduler::current_process()
{
    return *current_process_;
}

const std::size_t* Scheduler::schedule_next()
{
    if (processes_.get_processes().empty())
    {
        return nullptr;
    }
    if (processes_.get_processes().size() == 1)
    {
        current_process_ = processes_.get_processes().begin();
        return current_process_->current_stack_pointer();
    }
    
    if (current_process_ == processes_.get_processes().end())
    {
        current_process_ = processes_.get_processes().begin();
        return current_process_->current_stack_pointer();
    }
    else 
    {
        ++current_process_;
        if (current_process_ == processes_.get_processes().end())
        {
            current_process_ = processes_.get_processes().begin();
        }
    }
    
    return current_process_->current_stack_pointer(); 
}

} // namespace process     
} // namespace kernel     
} // namespace msos 

