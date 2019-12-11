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

def test_use_module_with_data_relocation():
    sut = utils.BinaryUnderTest(utils.find_binary("module_with_data"))
    sut.start()
    sut.expect("Module started", 1)
    sut.expect("Integer: -12345", 1)
    sut.expect("Constexpr integer: 21234", 1);
    sut.expect("Global integer: 177177", 1);
    sut.stop(1)

