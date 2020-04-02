# This file is part of MSOS project. This is simple OS for embedded development devices.
# Copyright (C) 20209 Mateusz Stadnik
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

def test_module_with_classes():
    sut = utils.BinaryUnderTest(utils.find_binary("spawn_process_from_filesystem"))
    sut.start()
    sut.expect("Device geometry")
    sut.expect("Size: 128")
    sut.expect("Read size: 1")
    sut.expect("Write size: 1")
    sut.expect("Erase size: 1")
    sut.expect("Creating file with name: /test.txt")
    sut.expect("File is not null")
    sut.expect("Content of file")
    sut.expect("Storing some data in file")
    sut.expect("Started testing ROMFS disk")
    sut.expect("Opening file /rom/test.txt")
    sut.expect("Content of file")
    sut.expect("This is simple file")
    sut.expect("Spawned child process")
    sut.expect("ImplementationA value: 10", 2)
    sut.expect("ImplementationB value: 15")
    sut.expect("ChildA value: 10")
    sut.expect("ImplementationA is printing")
    sut.expect("I am working! I_AM_B")
    sut.expect("ChildA is printing")
    sut.expect("ImplementationA is printing")
    sut.expect("Interface value: 10")
    sut.expect("Hello from Module")
    sut.stop(1)

