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

#include "msos/fs/read_only_filesystem.hpp"
#include "msos/drivers/i_driver.hpp"

namespace msos
{
namespace drivers
{

class DeviceFs : public fs::ReadOnlyFileSystem
{
public:
    static DeviceFs& get_instance();

    int mount(drivers::storage::BlockDevice& device) override;

    int umount() override;

    int stat(const eul::filesystem::path& path) override;

    std::unique_ptr<fs::IFile> get(const eul::filesystem::path& path) override;
    IDriver* get_driver(const eul::filesystem::path& path);

    std::vector<std::unique_ptr<fs::IFile>> list(const eul::filesystem::path& path) override;

    static bool register_driver(std::string_view name, IDriver& driver);
    std::string_view name() const override;
};

} // namespace drivers
} // namespace msos

#define REGISTER_DRIVER(name, driver) bool app_name ## _entry = msos::driver::DeviceFsz::register_driver(#name".bin", driver)
