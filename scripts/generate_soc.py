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
import os
import json
import collections

from pathlib import Path

parser = argparse.ArgumentParser(description = "CMake configuration generator based on KConfig")
parser.add_argument("-s", "--soc", dest="soc", action="store", help="SOC name", required=True)
parser.add_argument("-i", "--input", dest="input_directory", action="store", help="Path to input directory", required=True)
parser.add_argument("-o", "--output", dest="output_directory", action="store", help="Path to output directory", required=True)

args, rest = parser.parse_known_args()

def get_all_configs(soc, input_directory):
    next_config = soc
    configs = []

    while not next_config is None:
        with open(next_config) as config_file:
            configs.append(json.loads(config_file.read()))

        if "parent" in configs[-1]:
            next_config = None
            for path in Path(input_directory).rglob(configs[-1]["parent"] + ".json"):
                next_config = path
                break

            if next_config == None:
                print("Cannot find file: " + input_directory + "/../" + configs[-1]["parent"] + ".json")
                sys.exit(-1)
            del configs[-1]["parent"]
        else:
            next_config = None

    return configs

def merge(old, new):
    for k in new:
        if k in old:
            if isinstance(old[k], dict):
                merge(old[k], new[k])
        else:
            old[k] = new[k]

    return old

def write_node(node, prefix, file):
    print("node: ", node)
    for key in node:
        if prefix == "":
            pref = key
        else:
            pref = prefix + "_" + key
        print ("key: ", key)
        if not isinstance(node[key], dict):
            file.write("set (" + pref + " " + str(node[key]) + ")\n")
        else:
            write_node(node[key], pref, file)

def main():
    print("Searching: " + args.soc)
    soc_path = None
    for path in Path(args.input_directory).rglob(args.soc + ".json"):
        soc_path = path
        break

    if soc_path == None:
        sys.exit(-1)

    if not os.path.exists(args.output_directory):
        os.makedirs(args.output_directory)

    configs = get_all_configs(soc_path, args.input_directory)
    config = {}
    for c in configs:
        merge(config, c)
    print(config)

    cmake_soc_file = args.output_directory + "/soc_config.cmake"
    with open(cmake_soc_file, "w") as soc_config:
        write_node(config, "", soc_config)

main()
