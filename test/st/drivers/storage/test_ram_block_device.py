
# This file is part of MSOS project. This is simple OS for embedded development devices.
# Copyright (C) 2020 Mateusz Stadnik
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

def test_ram_block_device():
    sut = utils.BinaryUnderTest(utils.find_binary("ram_block_device_test"))
    sut.start()
    sut.expect("Device geometry:", 1)
    sut.expect("Size: 2048", 1)
    sut.expect("Read size: 1", 1)
    sut.expect("Write size: 1", 1)
    sut.expect("Erase size: 1", 1)
    sut.expect("Erase 24 bytes at 0x0", 1)
    sut.expect("Write Hello at address 0x4", 1)
    sut.expect("Read", 1)
    sut.expect("0 :", 1)
    sut.expect("0 :", 1)
    sut.expect("0 :", 1)
    sut.expect("0 :", 1)
    sut.expect("48 : H", 1)
    sut.expect("65 : e", 1)
    sut.expect("6c : l", 1)
    sut.expect("6c : l", 1)
    sut.expect("6f : o", 1)
    sut.expect("0 :", 1)
    sut.expect("0 :", 1)
    sut.expect("0 :", 1)
    sut.expect("0 :", 1)
    sut.expect("0 :", 1)
    sut.expect("0 :", 1)
    sut.expect("0 :", 1)
    sut.expect("0 :", 1)
    sut.expect("0 :", 1)
    sut.expect("0 :", 1)
    sut.stop(1)

