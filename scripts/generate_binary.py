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

import os
import argparse
import struct
import sys

from elftools.elf.elffile import ELFFile
from elftools.elf.sections import SymbolTableSection
from elftools.elf.relocation import RelocationSection
from elftools.elf.descriptions import describe_reloc_type
from jinja2 import Template, Environment, FileSystemLoader
from colorama import Fore, Style, init

from elf_parser import ElfParser
from printers import print_error, print_debug

def get_public_functions_from_symbols(symbols):
    filtered_symbols = {}
    for symbol_key in symbols:
        symbol = symbols[symbol_key]
        if symbol["type"] == "STT_FUNC":
            if symbol["visibility"] != "STV_HIDDEN" and (symbol["binding"] == "STB_GLOBAL" or symbol["binding"] == "STB_WEAK"):
                filtered_symbols[symbol_key] = symbol
            if symbol_key == "main" and (symbol["binding"] == "STB_GLOBAL" or symbol["binding"] == "STB_WEAK"):
                filtered_symbols[symbol_key] = symbol

    return filtered_symbols

def get_relocations(elf_filename):
    relocations = []
    with open(elf_filename, "rb") as elf_file:
        elf = ELFFile(elf_file)
        for section in elf.iter_sections():
            if not isinstance(section, RelocationSection):
                continue
            symbols = elf.get_section(section["sh_link"])
            for relocation in section.iter_relocations():
                if relocation["r_info_sym"] == 0:
                    continue
                relocation_data = {}
                relocation_data["offset"] = relocation["r_offset"]
                relocation_data["info"] = relocation["r_info"]
                relocation_data["info_type"] = describe_reloc_type(relocation["r_info_type"], elf)
                relocation_data["symbol"] = relocation["r_info_sym"]
                symbol = symbols.get_symbol(relocation_data["symbol"])
                if symbol["st_name"] == 0:
                    symbol_section = elf.get_section(symbol["st_shndx"])
                    relocation_data["symbol_name"] = symbol_section.name
                else:
                    relocation_data["symbol_name"] = symbol.name

                relocation_data["symbol_value"] = symbol["st_value"]

                relocations.append(relocation_data)
    return relocations

def get_symbol_names(symbols):
    symbol_names = []
    for key in symbols:
        symbol_names.append(key)
    return symbol_names

import subprocess

def process_section(elf, section_name, position):
    sections = elf.get_sections()
    if not section_name in sections:
        print_error("Trying to process not existing section: " + section_name)
        print_error("Available sections are: " + str(sections.keys()))
        sys.exit(-1)
    section = sections[section_name]
    if section["address"] != position:
        print_error(section_name + " must be placed at address " + str(position))
        sys.exit(-1)
    return section

def generate_module(module_name, elf_filename, objcopy_executable):
    elf = ElfParser(elf_filename)

    code_section = process_section(elf, ".text", 0)
    code_data = bytearray(code_section["data"])

    data_section = process_section(elf, ".data", code_section["address"] + code_section["size"])
    data_data = data_section["data"]

    bss_section = process_section(elf, ".bss", data_section["address"] + data_section["size"])

    print(Fore.YELLOW + "[INF]" + Style.RESET_ALL + "      Reading symbols")
    symbols = elf.get_symbols()

    print(Fore.YELLOW + "[INF]" + Style.RESET_ALL + "      Changing visibility of wrapped symbols")
    for symbol_name in symbols:
        if symbol_name.endswith("_dl_original"):
            subprocess.run([objcopy_executable + " -L " + symbol_name + " " + str(elf_filename)], shell=True)
    symbols = elf.get_symbols()
    print(Fore.YELLOW + "[INF]" + Style.RESET_ALL + "      Reading relocations")
    relocations = get_relocations(elf_filename)

    symbol_map = {}
    print(Fore.YELLOW + "[INF]" + Style.RESET_ALL + "      Processing symbols")
    for name, data in symbols.items():
        if not name or name == "$t" or name == "$d":
            continue

        if name == "main" or (data["binding"] == "STB_GLOBAL" and data["visibility"] != "STV_HIDDEN"):
            if data["section_index"] == "SHN_UNDEF":
                symbol_map[name] = "external"
            else:
                symbol_map[name] = "exported"
        else:
            if data["type"] == "STT_FILE":
                continue
            symbol_map[name] = "internal"

    print(Fore.YELLOW + "[INF]" + Style.RESET_ALL + "     External symbols: ")
    for name in filter(lambda elem: elem[1] == "external", symbol_map.items()):
        print("                    ", name[0])
    print("------------------------------------------------------")

    print(Fore.YELLOW + "[INF]" + Style.RESET_ALL + "     Exported symbols: ")
    for name in filter(lambda elem: elem[1] == "exported", symbol_map.items()):
        print("                    ", name[0])
    print("------------------------------------------------------")

    print(Fore.YELLOW + "[INF]" + Style.RESET_ALL + "     Internal symbols: ")
    for name in filter(lambda elem: elem[1] == "internal", symbol_map.items()):
        print("                    ", name[0])
    print("------------------------------------------------------")

    print(Fore.YELLOW + "[INF]" + Style.RESET_ALL + "      Processing relocations")
    local_relocations = []
    external_relocations = []
    data_relocation = []
    for relocation in relocations:
        offset = relocation["offset"]
        if relocation["info_type"] == "R_ARM_THM_CALL":
            continue
        elif relocation["info_type"] == "R_ARM_REL32":
            continue
        elif relocation["info_type"] == "R_ARM_GOT_BREL":
            symbol_visibility = symbol_map[relocation["symbol_name"]]
            if symbol_visibility == "external":
                external_relocations.append((relocation["symbol_name"], offset))
            elif symbol_visibility == "exported" or symbol_visibility == "internal":
                local_relocations.append((relocation["symbol_name"], offset))
        elif relocation["info_type"] != "R_ARM_ABS32":
           continue
           # print (Fore.RED + "Unknown relocation type. Please fix generate_binary.py")
           #  raise RuntimeError("Script not working for this binary")
    print (Fore.YELLOW + "[INF]" + Style.RESET_ALL + "      Local relocations:")
    for relocation in local_relocations:
        print ("                      ", relocation[0])

    print (Fore.YELLOW + "[INF]" + Style.RESET_ALL + "      External relocations:")
    for relocation in external_relocations:
        print ("                       ", relocation[0])


    relocation_to_index_map = {}
    index = 0
    total_relocations = 0
    for relocation in local_relocations + external_relocations:
        symbol_name = relocation[0]
        if not symbol_name in relocation_to_index_map:
            relocation_to_index_map[symbol_name] = index
            index += 1
            total_relocations += 1

    lot_offset = index
    data_relocations = []
    data_relocation_to_index_map = {}
    for relocation in relocations:
        if relocation["info_type"] == "R_ARM_ABS32":
            offset = int((relocation["offset"] - len(code_data)) / 4)
            symbol_name = relocation["symbol_name"]
            if not symbol_name in data_relocation_to_index_map:
                data_relocations.append((relocation["symbol_name"], offset + lot_offset))
                print ("adding ", symbol_name)
                data_relocation_to_index_map[symbol_name] = offset + lot_offset
                total_relocations += 1


    print (Fore.YELLOW + "[INF]" + Style.RESET_ALL + "     Data relocations:")
    for relocation in data_relocations:
        print ("                       ", relocation[0])


    for relocation in local_relocations + external_relocations:
        offset = relocation[1]
        old = struct.unpack_from("<I", code_data, offset)[0]
        new = relocation_to_index_map[relocation[0]] * 4
        struct.pack_into("<I", code_data, offset, new)

    print (Fore.YELLOW + "[INF]" + Style.RESET_ALL + " Creating image of module")

    # +---+---+---+---+
    # | M | S | D | L |
    # +---+---+---+---+
    # |   code size   |
    # +---+---+---+---+
    # |   data size   |
    # +---+---+---+---+
    # |   bss size    |
    # +---+---+---+---+
    # | n.rel | n.sym |
    # +---+---+---+---+
    # | s.siz | l.nam |
    # +---+---+---+---+
    # |     name      |
    # : aligned to 4  :
    # +---+---+---+---+
    # |  relocations  |
    # |   n-rel - 1   |

    # :     n-rel     :
    # +---+---+---+---+
    # |  symbol table |
    # |   s.siz - 1   |
    # :     s.siz     :
    # +---+---+---+---+

    # symbol table
    # +---+---+---+---+
    #

    image = bytearray("MSDL", "ascii")
    image += struct.pack("<III", len(code_data), len(data_data), len(bss_section["data"]))
    number_of_lot_relocations = index
    image += struct.pack("<HH", number_of_lot_relocations, total_relocations)
    name = bytearray(module_name + "\0", "ascii")
    image += name
    name_length = len(module_name) + 1
    if (name_length % 4):
        image += bytearray("\0" * (4 - (name_length % 4)), "ascii")

    symbol_to_index_map = {}
    symbol_index = 0
    for symbol in filter(lambda elem: elem[0] in relocation_to_index_map or elem[1] == "external" or elem[1] == "exported", symbol_map.items()):
        symbol_name = symbol[0]
        symbol_to_index_map.update({symbol_name: symbol_index})
        symbol_index = symbol_index + 1
        print("adding symbol with index: ", symbol_index)

    processed = []
    relocation_to_image = []
    for relocation in local_relocations + external_relocations:
        symbol, value = relocation
        if symbol in processed:
            continue
        rel = {}
        rel["index"] = relocation_to_index_map[symbol]
        rel["symbol"] = symbol_to_index_map[symbol]

        relocation_to_image.append(rel)
        #image += struct.pack("<II", relocation_to_index_map[symbol], symbol_to_index_map[symbol])
        processed.append(symbol)
    for relocation in data_relocations:
        symbol, value = relocation
        print ("processing relocation of: " + symbol)
        rel = {}
        rel["index"] = data_relocation_to_index_map[symbol]
        rel["symbol"] = symbol_to_index_map[symbol]

        relocation_to_image.append(rel)
        #image += struct.pack("<II", relocation_to_index_map[symbol], symbol_to_index_map[symbol])
    #image += struct.pack("<I", len(symbol_to_index_map))
    symbol_to_image = []
    for symbol in symbol_to_index_map:
        if symbol_map[symbol] == "internal":
            visibility = 0
        elif symbol_map[symbol] == "exported":
            visibility = 1
        elif symbol_map[symbol] == "external":
            visibility = 2

        if symbols[symbol]["section_index"] == code_section["index"]:
            section = 0
        elif symbols[symbol]["section_index"] == data_section["index"]:
            section = 1
        elif symbols[symbol]["section_index"] == bss_section["index"]:
            section = 1
        else:
            section = 0

        offset_to_next = len(symbol + "\0") + 4 + 2 + 2 + 4
        #image += struct.pack("<IHHI", offset_to_next,  visibility, section,symbol_to_index_map[symbol])
        #image += bytearray(symbol + "\0", "ascii")
        sym = {}
        sym["visibility"] = visibility
        sym["section"] = section
        sym["size"] = offset_to_next
        sym["name"] = symbol + "\0"
        sym["value"] = symbols[symbol]["value"]
        sym["index"] = symbol_to_index_map[symbol]
        symbol_to_image.append(sym)

    print ("Relocation table placed at: ", hex(len(image)))
    for rel in relocation_to_image:
        relocation_position = relocation_to_image.index(rel)
        sizeof_relocation = 8
        sizeof_symbol_table_size = 4

        offset_to_symbol_table = (len(relocation_to_image) - relocation_position) * sizeof_relocation + sizeof_symbol_table_size
        symbol_offset = 0
        for i in range(len(symbol_to_image)):
            if (symbol_to_image[i]["index"] == rel["symbol"]):
                break

            symbol_offset += symbol_to_image[i]["size"]
        print(rel["index"])
        image += struct.pack("<II", rel["index"], offset_to_symbol_table + symbol_offset)

    print ("Adding size of symbol table: ", len(symbol_to_index_map))
    print ("Symbol section placed at: ", hex(len(image)));
    image += struct.pack("<I", len(symbol_to_index_map))

    for sym in symbol_to_image:
        value = sym["value"]
        if sym["section"] == 1:
            value = sym["value"] - len(code_data)

        image += struct.pack("<IHHI", sym["size"], sym["visibility"], sym["section"], value)
        image += bytearray(sym["name"], "ascii")

    if (len(image) % 16):
        image += bytearray('\0' * (16 - (len(image) % 16)), "ascii")

    print ("code starts at: ", hex(len(image)))

    image += code_data
    image += data_data
    with open(module_name + ".bin", "wb") as file:
        file.write(image)

parser = argparse.ArgumentParser(description = "Relocable modules and shared libraries generator")
parser.add_argument("-o", "--output", dest="output_directory", action="store", help="Path to output file")
parser.add_argument("--module_name", dest="module_name", action="store", help="Module name")
parser.add_argument("-i", "--elf_filename", dest="elf_filename", action="store", help="Path to module ELF file")
parser.add_argument("--objcopy", dest="objcopy_executable", action="store", help="Path to objcopy executable")
parser.add_argument("--as_executable", dest="as_executable", action="store_true", help="Generate module as executable")
args, rest = parser.parse_known_args()

from pathlib import Path

def main():
    init()

    symbols_to_generate = []

    print(Fore.CYAN + "==========================================")
    print(Fore.CYAN + "=            MODULE_GENERATOR            =")
    print(Fore.CYAN + "==========================================")
    print(Style.RESET_ALL)

    print(Fore.YELLOW + "[INF]" + Style.RESET_ALL + " STEP 1. Initialization")
    if not args.elf_filename:
        print(Fore.RED + "[ERR] Please provide module ELF file (--elf_filename=<path_to_elf_file>)" + Style.RESET_ALL)
        return
    print(Fore.YELLOW + "[INF]" + Style.RESET_ALL + " STEP 2. Generating module: ", args.module_name, ", ELF: ", args.elf_filename)

    generate_module(args.module_name, args.elf_filename, args.objcopy_executable)
main()
