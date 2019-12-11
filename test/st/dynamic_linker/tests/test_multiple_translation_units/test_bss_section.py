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

import pytest

import utils

def test_bss_section():
    sut = utils.BinaryUnderTest(utils.find_binary("test_bss_section"))
    sut.start()
    sut.expect("Hello from bss test", 1)
    sut.expect("Array 1: 1, 2, 3, 4,", 1)
    sut.expect("Integer 1: 0", 1)
    sut.expect("Integer 2: 15", 1)
    sut.expect("Integer 3: 5", 1)
    sut.expect("Array 2: 5, 6, 19, 8,", 1)
    sut.stop(1)

