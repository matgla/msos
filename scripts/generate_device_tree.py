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
import os

def generate_cpp_header(output):
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

def generate_cmake_header(output):
    with open(output, "w") as file:
        file.write("########################################\n")
        file.write("#     FILE GENERATED AUTOMATICALLY     #\n")
        file.write("########################################\n\n")
        file.write("# This file is part of MSOS project.\n")
        file.write("# Copyright (C) 2020 Mateusz Stadnik\n")
        file.write("#\n")
        file.write("# This program is free software: you can redistribute it and/or modify\n")
        file.write("# it under the terms of the GNU General Public License as published by\n")
        file.write("# the Free Software Foundation, either version 3 of the License, or\n")
        file.write("# (at your option) any later version.\n")
        file.write("#\n")
        file.write("# This program is distributed in the hope that it will be useful,\n")
        file.write("# but WITHOUT ANY WARRANTY; without even the implied warranty of\n")
        file.write("# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\n")
        file.write("# GNU General Public License for more details.\n")
        file.write("#\n")
        file.write("# You should have received a copy of the GNU General Public License\n")
        file.write("# along with this program.  If not, see <https://www.gnu.org/licenses/>.\n\n")

def generate_cmake_library(output, drivers):
    with open(output, "a") as file:
        for driver_path, driver in drivers:
            if "path" in driver:
                file.write("set(ENABLE")
                for part in driver["path"].split("/"):
                    file.write("_" + part.upper())
            else:
                file.write("set(ENABLE_DRIVERS")
                for part in driver_path:
                    file.write("_" + part.upper())


            file.write(" ON CACHE BOOL \"\" FORCE)\n")
        file.write("add_library(device_tree STATIC)\n\n")
        file.write("target_sources(device_tree\n")
        file.write("    PUBLIC\n")
        file.write("         ${CMAKE_CURRENT_SOURCE_DIR}/device_tree.cpp)\n")
        file.write("\n")
        file.write("target_link_libraries(device_tree\n")
        file.write("    PUBLIC\n")
        file.write("        board\n")
        file.write("        msos_drivers\n")
        file.write("        hal\n")

        for driver_path, driver in drivers:
            if "path" in driver:
                library_name = ""
                for path_part in driver["path"].split("/"):
                    if len(library_name):
                        library_name += "_" + path_part
                    else:
                        library_name = path_part
            else:
                library_name = "msos_drivers"
                for path_part in driver_path:
                    if len(library_name):
                        library_name += "_" + path_part
                    else:
                        library_name = path_part
            file.write("        " + library_name + "\n")
        file.write(")\n")

        file.write("target_compile_options(device_tree\n")
        file.write("    PUBLIC\n")
        file.write("        $<$<COMPILE_LANGUAGE:CXX>:-std=c++2a>\n")
        file.write("        $<$<CONFIG:DEBUG>:-Og -g>\n")
        file.write("        $<$<CONFIG:RELEASE>:-Os>\n")
        file.write("    PRIVATE\n")
        file.write("        $<$<COMPILE_LANGUAGE:CXX>:-Werror -Wall -Wextra -Wpedantic -Wcast-align -Wunused -Wshadow -Wpointer-arith -Wcast-qual -Wno-missing-braces>\n")
        file.write(")\n")

def process_dict(obj, path, drivers):
    for next_key in obj:
        if next_key == "driver_name":
            drivers.append((path, obj))
            return

    for next_key in obj:
        path.append(next_key)
        process_next_key(obj, obj[next_key], path, drivers)

def process_next_key(parent, obj, path, drivers):
    if type(obj) is list:
        process_list_key(obj, path, drivers)
    if type(obj) is dict:
        process_dict(obj, path, drivers)

def process_list_key(json_file, path, drivers):
    objects = []
    for next_key_index, next_key in enumerate(json_file):
        obj = process_next_key(json_file, json_file[next_key_index], path, drivers)
        if type(obj) is list:
            objects.extend(obj)
        else:
            objects.append(obj)
    return objects

def process_next(json_file):
    drivers = []
    for key in json_file:
        path = [key]
        process_next_key(json_file, json_file[key], path, drivers)
    return drivers

def generate_cpp(output, drivers):
    with open(output, "a") as file:
            file.write("#include <msos/drivers/device_fs.hpp>\n")
            for driver_path, driver in drivers:
                if "path" in driver:
                    path = driver["path"]
                else:
                    path = "msos/drivers"
                file.write("#include <" + path)
                if not "path" in driver:
                    for path_part in driver_path:
                        file.write("/" + path_part)
                file.write("/" + driver["driver_name"] + ".hpp>\n")
            file.write("\n")
            for driver_path, driver in drivers:
                if "namespace" in driver:
                    namespace = driver["namespace"]
                else:
                    namespace = "msos::drivers"

                    for path_part in driver_path:
                        namespace += "::" + path_part
                file.write("static inline auto& create_" + driver["driver_name"] + "()\n")
                file.write("{\n")
                file.write("    static auto driver = " + namespace + "::create_" + driver["driver_name"] + "(")

                if "args" in driver:
                    i = 0
                    for arg in driver["args"]:
                        if i == 0:
                            i += 1
                            file.write(arg)
                        else:
                            file.write(", " + arg)

                file.write(");\n")
                file.write("    return driver;\n")
                file.write("}\n")
                file.write("\n")

            file.write("\n")
            for driver_path, driver in drivers:
                file.write("REGISTER_DRIVER(" + driver["name"] + ", create_" + driver["driver_name"] + "());\n")



parser = argparse.ArgumentParser(description = "Device tree library generator")
parser.add_argument("-i", "--input", dest="input_file", action="store", help="Device tree description file", required="true")
parser.add_argument("-o", "--output", dest="output_directory", action="store", help="Device tree output directory", required="true")

args, rest = parser.parse_known_args()

if not os.path.exists(args.output_directory):
    os.makedirs(args.output_directory)

cmakelists_file = args.output_directory + "/CMakeLists.txt"
cpp_file = args.output_directory + "/device_tree.cpp"
with open(args.input_file) as description_file:
    desc = json.loads(description_file.read())
    drivers = process_next(desc)

    generate_cmake_header(cmakelists_file)
    generate_cmake_library(cmakelists_file, drivers)

    generate_cpp_header(cpp_file)
    generate_cpp(cpp_file, drivers)
