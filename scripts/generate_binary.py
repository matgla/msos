#!/usr/bin/python3

import os
import argparse

from elftools.elf.elffile import ELFFile
from elftools.elf.sections import SymbolTableSection
from elftools.elf.relocation import RelocationSection
from elftools.elf.descriptions import describe_reloc_type


def get_symbols(elf_filename):
    symbols = {}
    with open(elf_filename, "rb") as elf_file:
        elf = ELFFile(elf_file)
        for section in elf.iter_sections():
            if not isinstance(section, SymbolTableSection):
                continue
            for symbol in section.iter_symbols():
                data = {}
                data["type"] = symbol["st_info"]["type"]
                data["binding"] = symbol["st_info"]["bind"]
                data["name"] = symbol["st_name"]
                data["value"] = symbol["st_value"]
                data["size"] = symbol["st_size"]
                data["section_index"] = symbol["st_shndx"]
                data["visibility"] = symbol["st_other"]["visibility"]
                symbols[symbol.name] = data
    return symbols

def get_public_functions_from_symbols(symbols):
    filtered_symbols = {}
    for symbol_key in symbols:
        symbol = symbols[symbol_key]
        if symbol["type"] == "STT_FUNC":
            if symbol["binding"] == "STB_GLOBAL" or symbol["binding"] == "STB_WEAK":
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

def get_sections(elf_filename):
    sections = {}
    with open(elf_filename, "rb") as elf_file:
        elf = ELFFile(elf_filename)
        for section in elf.iter_sections():
            section_data = {}
            section_data["name"] = section.name
            section_data["address"] = section["sh_addr"]
            section_data["type"] = section["sh_type"]
            section_data["flags"] = section["sh_flags"]
            section_data["offset"] = section["sh_offset"]
            section_data["size"] = section["sh_size"]
            section_data["link"] = section["sh_link"]
            section_data["info"] = section["sh_info"]
            section_data["addralign"] = section["sh_addralign"]
            section_data["size"] = section["sh_entsize"]
            section_data["data"] = section.data()
            sections[section.name] = section_data

    return sections

def get_symbol_names(symbols):
    symbol_names = []
    for key in symbols:
        symbol_names.append(key)
    return symbol_names

import subprocess

def wrap_symbols(symbol_names, filename, objcopy_executable):
    if (not objcopy_executable):
        print ("objcopy executable not provided")
    print ("wrapping symbols in: ", filename)
    for symbol in symbol_names:
        if symbol.endswith("_dl_original"):
            continue

        print ("Renaming symbol: ", symbol, ", to: ", symbol + "_dl_original")
        print ("Calling: ", objcopy_executable, filename, "--redefine-sym", " " + symbol + "=" + symbol + "_dl_original")
        subprocess.run([objcopy_executable + " --redefine-sym " + symbol + "=" + symbol + "_dl_original " + str(filename)], shell=True)



parser = argparse.ArgumentParser(description = "Relocable modules and shared libraries generator")
parser.add_argument("-i", "--input", dest="input_directory", action="store", help="Path to input file")
parser.add_argument("-o", "--output", dest="output_directory", action="store", help="Path to output file")
parser.add_argument("--objcopy", dest="objcopy_executable", action="store", help="Path to objcopy executable")

args, rest = parser.parse_known_args()

print("Filename: ", args.input_directory)

#symbols = get_symbols(args.elf_filename)
#print(get_public_functions_from_symbols(symbols))
#relocations = get_relocations(args.elf_filename)
#print(relocations)

from pathlib import Path

for file in Path(args.input_directory).rglob("*.o"):
    print (file)
    symbols = get_symbols(file)
    public_symbols = get_public_functions_from_symbols(symbols)
    print (public_symbols)
    symbols_to_rename = get_symbol_names(public_symbols)
    wrap_symbols(symbols_to_rename, file, args.objcopy_executable)
    relocations = get_relocations(file)
    #print(relocations)

