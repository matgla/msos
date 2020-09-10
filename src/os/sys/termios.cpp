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

#include "msos/os/sys/termios.h"

#include <cerrno>

int tcgetattr(int fildes, struct termios *termios_p)
{
    return ioctl(fildes, TIOCGETA, termios_p);
}

int tcsetattr(int fd, int optional_actions,
       const struct termios *t)
{
    switch (optional_actions)
    {
        case TCSANOW:
            return (ioctl(fd, TIOCSETA, t));
        case TCSADRAIN:
            return (ioctl(fd, TIOCSETAW, t));
        case TCSAFLUSH:
            return (ioctl(fd, TIOCSETAF, t));
        default:
            errno = EINVAL;
            return (-1);
    }
}
