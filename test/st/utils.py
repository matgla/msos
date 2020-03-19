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
import pexpect

import configparser
import sys

def find_binary(binary_name):
    config = configparser.ConfigParser()
    config.read_file(open("config.ini"))

    binary_path = config["BINARIES"][binary_name]
    return binary_path

class BinaryUnderTest:
    def __init__(self, binary_path):
        self.binary_path = binary_path

    def start(self):
        self.executable = pexpect.spawnu("qemu-system-arm -kernel " + \
                                         self.binary_path + \
                                         " -machine stm32_black_pill -serial stdio")
    def stop(self, timeout):
        if (self.executable):
            self.executable.expect("TEST DONE", timeout)
            self.executable.sendline("quit")
        else:
            raise "Executable not started"

    def expect(self, expectation, timeout):
        if (self.executable):
            self.executable.expect(expectation, timeout)
        else:
            raise "Executable not started"
