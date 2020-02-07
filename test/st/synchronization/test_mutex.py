
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

def test_mutex_synchronization():
    sut = utils.BinaryUnderTest(utils.find_binary("mutex_test"))
    sut.start()
    sut.expect("Parent is starting", 1)
    sut.expect("Parent is going to sleep", 1)
    sut.expect("Child is starting", 1)
    sut.expect("Parent finished waiting", 1)
    # sut.expect("Parent is working", 1)
    sut.expect("Child is going to sleep", 1)
    sut.expect("Parent is working", 1)
    sut.expect("Child finished waiting", 1)
    sut.expect("Child is working", 1)
    sut.stop(1)

