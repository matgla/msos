#!/usr/bin/python3

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

import argparse
import sys

from kconfig.kconfiglib import Kconfig, Symbol, Choice, MENU, COMMENT

parser = argparse.ArgumentParser(description = "CMake configuration generator based on KConfig")
parser.add_argument("-i", "--input", dest="input_file", action="store", help="Path to input file", required=True)
parser.add_argument("-o", "--output", dest="output_file", action="store", help="Path to output file", required=True)
parser.add_argument("-k", "--kconfig", dest="kconfig", action="store", help="Path to kconfig file", required=True)

args, rest = parser.parse_known_args()

def main():
    kconf = Kconfig(args.kconfig)
    kconf.load_config(args.input_file)
    with open(args.output_file, "w") as output:
        for node in kconf.unique_defined_syms:
            if node.user_value:
                output.write("set(" + node.name + " " + str(node.user_value) + " CACHE INTERNAL \"\")\n")
main()
