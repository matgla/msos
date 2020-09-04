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

#include "msos/fs/ramfs_file.hpp"

#include <eul/utils/unused.hpp>

namespace msos
{
namespace fs
{

RamfsFile::RamfsFile(RamFsData& data)
    : data_{data}
    , position_(0)
{

}

ssize_t RamfsFile::read(DataType data)
{
    if (position_ >= data_.data().size())
    {
        return 0;
    }
    size_t len = (data_.data().size() - position_) > static_cast<size_t>(data.size()) ? data.size() : data_.data().size() - position_;

    std::copy(data_.data().begin() + position_, data_.data().end(), data.begin());
    position_ += len;
    return len;
}

ssize_t RamfsFile::write(const ConstDataType data)
{
    std::copy(data.begin(), data.end(), std::back_inserter(data_.data()));
    return data.size();
}

off_t RamfsFile::seek(off_t offset, int base) const
{
    UNUSED2(offset, base);
    return 0;
}

int RamfsFile::close()
{
    return 0;
}

int RamfsFile::sync()
{
    return 0;
}

off_t RamfsFile::tell() const
{
    return 0;
}

ssize_t RamfsFile::size() const
{
    return 0;
}

std::string_view RamfsFile::name() const
{
    return data_.filename();
}

std::unique_ptr<IFile> RamfsFile::clone() const
{
    return std::make_unique<RamfsFile>(data_);
}

const char* RamfsFile::data() const
{
    return reinterpret_cast<const char*>(data_.data().data());
}

void RamfsFile::stat(struct stat& s) const
{
    printf("RamFs stat\n");
    s.st_mode = 0;
    if (data_.is_directory())
    {
        printf ("DIRECT\n");
        s.st_mode |= S_IFDIR;
    }
    else
    {
        printf ("FILE\n");
        s.st_mode |= S_IFREG;
    }
}

} // namespace fs
} // namespace msos
