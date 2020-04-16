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

#!/usr/bin/python3

import argparse
import json

def generate_header(output):
    with open(output, "w") as file:
        file.write("/*************************************/\n")
        file.write("/*    FILE GENERATED AUTOMATICALLY   */\n")
        file.write("/*************************************/\n\n")
        file.write("// This file is part of MSOS project.\n")
        file.write("// Copyright (C) 2020 Mateusz Stadnik\n")
        file.write("//\n")
        file.write("// This program is free software: you can redistribute it and/or modify\n")
        file.write("// it under the terms of the GNU General Public License as published by\n")
        file.write("// the Free Software Foundation, either version 3 of the License, or\n")
        file.write("// (at your option) any later version.\n")
        file.write("//\n")
        file.write("// This program is distributed in the hope that it will be useful,\n")
        file.write("// but WITHOUT ANY WARRANTY; without even the implied warranty of\n")
        file.write("// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\n")
        file.write("// GNU General Public License for more details.\n")
        file.write("//\n")
        file.write("// You should have received a copy of the GNU General Public License\n")
        file.write("// along with this program.  If not, see <https://www.gnu.org/licenses/>.\n\n")

        file.write("#pragma once\n\n")

def generate_api(api_file, output):
    generate_header(output)
    with open(api_file) as codes_json:
        api = json.loads(codes_json.read())

        with open(output, "a") as file:
            file.write("enum SymbolCode\n")
            file.write("{\n")
            for lib in api:
                for sym in api[lib]:
                    file.write("    " + lib + "_" + sym + " = " + str(api[lib][sym]) + ",\n")
            file.write("};\n\n")


parser = argparse.ArgumentParser(description = "Relocable modules and shared libraries generator")
parser.add_argument("--api", dest="api_file", action="store", help="API descryption file", required="true")
parser.add_argument("--output", "-o", dest="output", action="store", help="Generated file destination", required="true")
args, rest = parser.parse_known_args()

generate_api(args.api_file, args.output)
