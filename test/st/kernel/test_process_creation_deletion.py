
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

def test_process_creation_deletion():
    sut = utils.BinaryUnderTest(utils.find_binary("process_creation_deletion_test"))
    sut.start()
    sut.expect("Hello from Kernel", 1)
    sut.expect("Parent", 1)
    sut.expect("Parent going to sleep", 1)
    sut.expect("Child process started", 1)
    sut.expect("Child is going to sleep", 1)
    sut.expect("Parent: 0", 1)
    sut.expect("Child Done", 1)
    sut.expect("Child has additional print", 1)
    sut.expect("Parent: 2", 1)
    sut.expect("Child A", 1)
    sut.expect("Child B", 1)
    sut.expect("Parent:", 1)
    sut.expect("Child A finished", 1)
    sut.expect("Process exited with code: 0", 1)
    sut.expect("Child B", 1)
    sut.expect("Child B", 1)
    sut.expect("B finished", 1)
    sut.expect("Process exited with code: 0", 1)
    sut.expect("Child C is starting", 1)
    sut.expect("Parent:", 1)

    sut.stop(1)

