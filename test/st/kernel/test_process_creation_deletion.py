
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
    sut.expect_async("Parent going to sleep", 1)
    sut.expect_async("Child process started", 1)
    sut.expect_async("Child is going to sleep", 1)
    sut.expect_async("Child Done", 1)
    sut.expect_async("Child has additional print", 1)
    sut.expect_async("Child A", 1)
    sut.expect_async("Child A finished", 1)
    sut.expect_async("Child B", 1)
    sut.expect_async("B finished", 1)
    sut.expect_async("Child C is starting: a", 1)

    sut.stop(1)

