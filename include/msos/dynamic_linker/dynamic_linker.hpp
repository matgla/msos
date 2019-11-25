#pragma once

#include <string_view>
#include <list>

#include "msos/dynamic_linker/module.hpp"
#include "msos/dynamic_linker/symbol.hpp"
#include "msos/dynamic_linker/relocation.hpp"
#include "msos/usart_printer.hpp"

namespace msos
{
namespace dl
{

class Module;

struct ModuleInfo
{
    const uint32_t main_address;
    const uint32_t lot_address;
};

class LoadedModuleList
{

};

class LoadedModuleNode
{
public:

private:
    const Module* module_header_;
    ModuleInfo module_info_;
};

enum class LoadingStatus
{
    ModuleCookieValidationFailed,
    Ok
};

class DynamicLinker
{
public:
    template <typename Environment>
    ModuleInfo load_module(const uint32_t module_address, const Environment& environment)
    {
        writer << "Loading module from: 0x" << hex << module_address << endl;
        const Module* module = reinterpret_cast<const Module*>(module_address);

        if (module->cookie() != "MSDL")
        {
            return {};//;LoadingStatus::ModuleCookieValidationFailed;
        }

        const uint32_t relocation_section_address = module_address + module->size();
        writer << "Relocations section: 0x" << hex << relocation_section_address << endl;
        const uint32_t relocation_section_size = get_relocations_size(module, relocation_section_address);
        writer << "Relocation section size: 0x" << relocation_section_size << endl;
        const uint32_t symbol_section_address = relocation_section_address + relocation_section_size;
        writer << "Symbol section: 0x" << hex << symbol_section_address << endl;
        const uint32_t symbol_section_size = get_symbols_size(symbol_section_address);
        writer << "Symbol section size: 0x" << hex << symbol_section_size << endl;

        const uint32_t not_aligned_code_address = symbol_section_address + symbol_section_size;
        writer << "Not aligned code address: 0x" << not_aligned_code_address << endl;
        const uint32_t code_address = not_aligned_code_address % 16 ? not_aligned_code_address + (16 - (not_aligned_code_address % 16)) : not_aligned_code_address;
        writer << "Code: 0x" << code_address << endl;
        const uint32_t rodata_address = code_address + module->code_size();
        writer << "Rodata: 0x" << rodata_address << endl;
        // const uint32_t data_address = rodata_address + module->rodata_size();

        const uint32_t size_of_lot = get_size_of_lot(module, relocation_section_address);
        writer << "Size of LOT: " << dec << size_of_lot << endl;

        uint32_t* lot = new uint32_t[2000];
        writer << "Address of LOT: 0x" << hex << reinterpret_cast<uint32_t>(lot) << endl;
        delete[] lot;
        process_relocations(module, relocation_section_address, lot, code_address, rodata_address, environment);

        const auto* main = find_symbol(symbol_section_address, "main");
        writer << "main function offset: " << main->offset() << ", lot: 0x" << hex << reinterpret_cast<uint32_t>(lot)  << endl;

        return {code_address + main->offset() - 1, reinterpret_cast<uint32_t>(&lot[0])};
    }

    const Symbol* find_symbol(const uint32_t address, const std::string_view& symbol_name)
    {
        const uint32_t number_of_symbols = *reinterpret_cast<uint32_t*>(address);
        const Symbol* symbol = reinterpret_cast<Symbol*>(address + 4);
        for (uint32_t i = 0; i < number_of_symbols; ++i)
        {
            if (symbol->name() == symbol_name)
            {
                return symbol;
            }
            symbol = symbol->next();
        }
        return nullptr;
    }
private:

    uint32_t get_size_of_lot(const Module* module, uint32_t address)
    {
        uint32_t size_of_lot = 0;
        const Relocation* relocation = reinterpret_cast<const Relocation*>(address);

        for (int i = 0; i < module->number_of_relocations(); ++i)
        {
            const Symbol* symbol = relocation->symbol();

            if (symbol->visibility() == SymbolVisibility::internal || symbol->visibility() == SymbolVisibility::exported)
            {
                if (symbol->section() == Section::rodata)
                {
                    ++size_of_lot;
                }
            }
            else if (symbol->visibility() == SymbolVisibility::external)
            {
                ++size_of_lot;
            }
            relocation = relocation->next();
        }
        return size_of_lot;
    }

    template <typename Environment>
    void process_relocations(const Module* module, uint32_t address, uint32_t* lot, uint32_t code, uint32_t rodata, const Environment& env)
    {
        for (int i = 0; i < module->number_of_relocations(); ++i)
        {
            const Relocation* relocation = reinterpret_cast<const Relocation*>(address);
            address += relocation->size();
            const Symbol* symbol = relocation->symbol();

            if (symbol->visibility() == SymbolVisibility::internal || symbol->visibility() == SymbolVisibility::exported)
            {
                if (symbol->section() == Section::rodata)
                {
                    lot[i] = rodata;
                }
                else if (symbol->section() == Section::code)
                {
                }
            }
            else if (symbol->visibility() == SymbolVisibility::external)
            {
                writer << "Searching symbol address for: " << symbol->name() << endl;
                const auto* env_symbol = env.find_symbol(symbol->name());
                if (env_symbol)
                {
                    writer << "Symbol found in env at address 0x" << hex << env_symbol->address();
                    lot[i] = env_symbol->address();
                }
            }
        }
    }

    uint32_t get_relocations_size(const Module* module, const uint32_t address)
    {
        uint32_t size = 0;
        const Relocation* relocation = reinterpret_cast<const Relocation*>(address);

        for (int i = 0; i < module->number_of_relocations(); ++i)
        {
            writer << "Relocation: " << relocation->symbol()->name() << ", index: " << dec << relocation->index() << endl;
            size += relocation->size();
            relocation = relocation->next();
        }
        return size;
    }

    uint32_t get_symbols_size(const uint32_t address)
    {
        uint32_t size = 4;
        uint32_t number_of_symbols = *reinterpret_cast<uint32_t*>(address);

        const Symbol* symbol = reinterpret_cast<const Symbol*>(address + 4);

        for (uint32_t i = 0; i < number_of_symbols; ++i)
        {
            writer << "symbol : " << symbol->name() << ", v: " << to_string(symbol->visibility()) << ", section: " << to_string(symbol->section()) << "offset: " << symbol->offset() << endl;
            size += symbol->size();
            symbol = symbol->next();
        }
        return size;
    }

private:
    std::list<LoadedModuleNode> loaded_modules_;
};

} // namespace dl
} // namespace msos

