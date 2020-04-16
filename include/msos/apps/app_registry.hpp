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

#pragma once

#include <string_view>

#include "msos/fs/i_filesystem.hpp"

namespace msos
{
namespace apps
{

struct AppEntry
{
    AppEntry() : name("/"), address(0xdeadbeef)
    {

    }
    AppEntry(const std::string_view& n, std::size_t a)
        : name{n}
        , address{a}
    {
    }


    std::string_view name;
    std::size_t address;
};

class AppRegistry : public fs::IFileSystem
{
public:
    static AppRegistry& get_instance();

    int mount(drivers::storage::BlockDevice& device) override;

    int umount() override;

    int create() override;

    int mkdir(const eul::filesystem::path& path, int mode) override;

    int remove(const eul::filesystem::path& path) override;

    int stat(const eul::filesystem::path& path) override;

    std::unique_ptr<fs::IFile> get(const eul::filesystem::path& path) override;
    std::unique_ptr<fs::IFile> create(const eul::filesystem::path& path) override;

    std::vector<std::unique_ptr<fs::IFile>> list(const eul::filesystem::path& path) override;

    static bool register_executable(std::string_view name, std::size_t address);
    std::string_view name() const override;

private:
    std::vector<AppEntry> apps_;
};

} // namespace apps
} // namespace msos

#define REGISTER_APP(name, address) bool app_name ## _entry = msos::apps::AppRegistry::register_executable(#name".bin", reinterpret_cast<std::size_t>(address))
