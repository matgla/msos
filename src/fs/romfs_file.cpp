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

#include "msos/fs/romfs_file.hpp"

#include <cstring>

#include <eul/utils/unused.hpp>

namespace msos
{
namespace fs
{

RomFsFile::RomFsFile(const romfs::FileHeader& fh)
    : file_(fh)
    , position_(0)

{
}

ssize_t RomFsFile::read(DataType data)
{
    if (position_ >= file_.get_size())
    {
        return 0;
    }
    size_t len = (file_.get_size() - position_) > static_cast<size_t>(data.size()) ? data.size() : file_.get_size() - position_;

    std::memcpy(data.data(), file_.data() + position_, len);
    position_ += len;
    return len;
}

ssize_t RomFsFile::write(const ConstDataType data)
{
    UNUSED1(data);
    return -1;
}

off_t RomFsFile::seek(off_t offset, int base) const
{
    UNUSED2(offset, base);
    return 0;
}

int RomFsFile::close()
{
    return 0;
}

int RomFsFile::sync()
{
    return 0;
}

off_t RomFsFile::tell() const
{
    return 0;
}

ssize_t RomFsFile::size() const
{
    return 0;
}

std::string_view RomFsFile::name() const
{
    return file_.get_name();
}

std::unique_ptr<IFile> RomFsFile::clone() const
{
    return std::make_unique<RomFsFile>(file_);
}

const char* RomFsFile::data() const
{
    return reinterpret_cast<const char*>(file_.data());
}

} // namespace fs
} // namespace msos
