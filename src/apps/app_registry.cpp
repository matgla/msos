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

#include "msos/apps/app_registry.hpp"

#include <algorithm>

#include <eul/utils/unused.hpp>

#include "msos/apps/app_file.hpp"
#include "msos/usart_printer.hpp"

namespace msos
{
namespace apps
{

static AppRegistry a;
static std::vector<AppEntry> apps_;
static UsartWriter writer;

AppRegistry& AppRegistry::get_instance()
{
    return a;
}

int AppRegistry::mount(drivers::storage::BlockDevice& device)
{
    UNUSED1(device);
    return 0;
}

int AppRegistry::umount()
{
    return 0;
}

int AppRegistry::create()
{
    return 0;
}

int AppRegistry::mkdir(std::string_view path, int mode)
{
    UNUSED2(path, mode);
    return 0;
}

int AppRegistry::remove(std::string_view path)
{
    UNUSED1(path);
    return 0;
}

int AppRegistry::stat(std::string_view path)
{
    UNUSED1(path);
    return 0;
}

std::unique_ptr<fs::IFile> AppRegistry::get(std::string_view path)
{
    auto it = std::find_if(apps_.begin(), apps_.end(), [path](auto& entry){
        writer << entry.name  << " == " << path;
        return entry.name == path;
    });
    if (it != apps_.end())
    {
        return std::make_unique<AppFile>(*it);
    }
    if (path == "" || path == "/")
    {
        return std::make_unique<AppFile>();
    }
    return nullptr;
}

std::unique_ptr<fs::IFile> AppRegistry::create(std::string_view path)
{
    UNUSED1(path);
    return nullptr;
}

std::vector<std::unique_ptr<fs::IFile>> AppRegistry::list(std::string_view path)
{
    UNUSED1(path); // single directory
    std::vector<std::unique_ptr<fs::IFile>> files;
    for (const auto& app : apps_)
    {
        files.push_back(std::make_unique<AppFile>(app));
    }
    return files;
}

bool AppRegistry::register_executable(std::string_view name, std::size_t address)
{
    apps_.push_back(AppEntry{name, address});
    return true;
}

std::string_view AppRegistry::name() const
{
    return "AppFs";
}


} // namespace apps
} // namespace msos

