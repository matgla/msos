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

#include <sys/ioctl.h>

extern "C"
{

typedef struct audio_buf_info {
    int fragments;
    int fragstotal;
    int fragsize;
    int bytes;
} audio_buf_info;

#define SNDCTL_DSP_SPEED        _IOWR('P', 2, int)
#define SNDCTL_DSP_GETBLKSIZE   _IOWR('P', 4, int)
#define SNDCTL_DSP_SETFMT       _IOWR('P', 5, int)
#define SNDCTL_DSP_CHANNELS     _IOWR('P', 6, int)
#define SNDCTL_DSP_SETFRAGMENT  _IOWR('P', 10, int)

#define SNDCTL_DSP_GETOSPACE    _IOR('P', 12, audio_buf_info)

#define SNDCTL_DSP_NONBLOCK     _IO('P', 14)

#define AFMT_U8 0x00000008

}
