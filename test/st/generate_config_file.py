# This file is part of MSOS project. This is simple OS for embedded development devices.
# Copyright (C) 2019 Mateusz Stadnik
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <https://www.gnu.org/licenses/>.

import sys
import configparser

data = sys.argv[1]

binaries = data.split(";")

config = configparser.ConfigParser()
config.add_section("BINARIES")
config["BINARIES"] = {}

for binary in binaries:
    if len(binary) == 0:
        continue
    binary_data = binary.split(":")
    name = binary_data[0]
    path = binary_data[1]
    config["BINARIES"].update({name: path})


with open("config.ini", "w") as f:
    config.write(f)
