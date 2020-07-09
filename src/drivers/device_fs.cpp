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

#include "msos/drivers/device_fs.hpp"

#include <list>

#include <eul/utils/unused.hpp>

#include "msos/fs/file_base.hpp"

#include "msos/usart_printer.hpp"

namespace msos
{
namespace drivers
{
struct DeviceFsRootFile : public fs::FileBase
{
public:
    ssize_t read(DataType data) override
    {
        UNUSED1(data);
        return 0;
    }
    ssize_t write(const ConstDataType data) override
    {
        UNUSED1(data);
        return 0;
    }
    off_t seek(off_t offset, int base) const override
    {
        UNUSED2(offset, base);
        return 0;
    }

    int close() override
    {
        return 0;
    }

    int sync() override
    {
        return 0;
    }

    off_t tell() const override
    {
        return 0;
    }

    ssize_t size() const override
    {
        return 0;
    }

    std::string_view name() const override
    {
        return "";
    }

    std::unique_ptr<IFile> clone() const override
    {
        return std::make_unique<DeviceFsRootFile>();
    }

    const char* data() const override
    {
        return nullptr;
    }
};

static int instances = 0;

DeviceFs& DeviceFs::get_instance()
{
    static DeviceFs a;
    return a;
}

int DeviceFs::mount(drivers::storage::BlockDevice& device)
{
    UNUSED1(device);
    return 0;
}

int DeviceFs::umount()
{
    return 0;
}

int DeviceFs::stat(const eul::filesystem::path& path)
{
    UNUSED1(path);
    return 0;
}

IDriver* DeviceFs::get_driver(const eul::filesystem::path& path)
{
    for (auto& entry : drivers)
    {
        if (path.filename() == entry.path())
        {
            return entry.driver();
        }
    }
    return nullptr;
}

std::unique_ptr<fs::IFile> DeviceFs::get(const eul::filesystem::path& path, int flags)
{
    if (path.native().empty() || path.native() == "/")
    {
        return std::make_unique<DeviceFsRootFile>();
    }

    auto* driver = get_driver(path);
    if (driver)
    {
        return driver->file(path.native(), flags);
    }

    return nullptr;
}

std::vector<std::unique_ptr<fs::IFile>> DeviceFs::list(const eul::filesystem::path& path)
{
    UNUSED1(path);
    std::vector<std::unique_ptr<fs::IFile>> files;
    for (auto& entry : drivers)
    {
        if (entry.driver()->file(entry.path(), 0) != nullptr)
        {
            files.push_back(entry.driver()->file(entry.path(), 0));
        }
    }
    return files;
}

int DeviceFs::add_driver(std::string_view name, IDriver& driver)
{
    drivers.push_back(DriverEntry{name, &driver});
    return instances++;
}

bool DeviceFs::register_driver(std::string_view name, IDriver& driver)
{
    get_instance().add_driver(name, driver);
    return true;
}


const std::list<DriverEntry>& DeviceFs::get_drivers() const
{
    return drivers;
}


std::list<DriverEntry>& DeviceFs::get_drivers()
{
    return drivers;
}


std::string_view DeviceFs::name() const
{
    return "DevFs";
}


} // namespace drivers
} // namespace msos
