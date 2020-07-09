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

AppRegistry& AppRegistry::get_instance()
{
    static AppRegistry a;
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

int AppRegistry::create(int flags)
{
    UNUSED1(flags);
    return 0;
}

int AppRegistry::mkdir(const eul::filesystem::path& path, int mode)
{
    UNUSED2(path, mode);
    return 0;
}

int AppRegistry::remove(const eul::filesystem::path& path)
{
    UNUSED1(path);
    return 0;
}

int AppRegistry::stat(const eul::filesystem::path& path)
{
    UNUSED1(path);
    return 0;
}

std::unique_ptr<fs::IFile> AppRegistry::get(const eul::filesystem::path& path, int flags)
{
    UNUSED1(flags);
    if (path.native() == "" || path.native() == "/")
    {
        return std::make_unique<AppFile>();
    }

    for (const auto& app : apps_)
    {
        if (app.name == path.native())
        {
            return std::make_unique<AppFile>(app);
        }
    }

    return nullptr;
}

std::unique_ptr<fs::IFile> AppRegistry::create(const eul::filesystem::path& path, int flags)
{
    UNUSED2(path, flags);
    return nullptr;
}

std::vector<std::unique_ptr<fs::IFile>> AppRegistry::list(const eul::filesystem::path& path)
{
    UNUSED1(path); // single directory
    std::vector<std::unique_ptr<fs::IFile>> files;
    for (const auto& app : apps_)
    {
        files.push_back(std::make_unique<AppFile>(app));
    }
    return files;
}

bool AppRegistry::register_executable(std::string_view name, std::size_t address, bool autostart)
{
    get_instance().apps_.push_back(AppEntry{name, address, autostart});
    return true;
}

std::string_view AppRegistry::name() const
{
    return "AppFs";
}

const std::vector<AppEntry>& AppRegistry::get_apps() const
{
    return apps_;
}



} // namespace apps
} // namespace msos

