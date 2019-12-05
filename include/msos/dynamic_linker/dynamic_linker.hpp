#pragma once

#include <string_view>
#include <optional>
#include <eul/container/static_vector.hpp>

#include <gsl/span>

#include "msos/dynamic_linker/module.hpp"
#include "msos/dynamic_linker/module_header.hpp"
#include "msos/dynamic_linker/symbol.hpp"
#include "msos/dynamic_linker/relocation.hpp"
#include "msos/dynamic_linker/module_data.hpp"
#include "msos/usart_printer.hpp"
#include "msos/dynamic_linker/loaded_module.hpp"

namespace msos
{
namespace dl
{

extern "C"
{
    int call_external(uint32_t address);
}

class Module;

struct ModuleInfo
{
    const uint32_t main_address;
    const uint32_t lot_address;
};

constexpr int LoadingModeCopyData = 0x01;
constexpr int LoadingModeCopyText = 0x02;
constexpr int LoadingModeCopyRodata = 0x04;

class DynamicLinker
{
public:
    DynamicLinker() : modules_{}
    {
        writer << "Modules size: " << modules_.size() << endl;
    }

    void unload_module(const LoadedModule* module)
    {
        auto module_it = std::find_if(modules_.begin(), modules_.end(), [module](const auto& loaded_module) {
            return &loaded_module == module;
        });
        if (module_it != modules_.end())
        {
           //  modules_.erase(module_it);
        }
    }

    template <typename Environment>
    const LoadedModule* load_module(const uint32_t module_address, const int mode, const Environment& environment)
    {
        writer << "Loading module from: 0x" << hex << module_address << endl;
        const ModuleHeader& header = *reinterpret_cast<const ModuleHeader*>(module_address);

        if (header.cookie() != "MSDL")
        {
            return nullptr;//;LoadingStatus::ModuleCookieValidationFailed;
        }

        writer << "Module header " << header.name() << ", size: " << dec << header.size() << endl;
        const uint32_t relocation_section_address = module_address + header.size();
        writer << "Relocations section: 0x" << hex << relocation_section_address << endl;
        const uint32_t relocation_section_size = get_relocations_size(header, relocation_section_address);
        writer << "Relocation section size: 0x" << relocation_section_size << endl;
        const uint32_t symbol_section_address = relocation_section_address + relocation_section_size;
        writer << "Symbol section: 0x" << hex << symbol_section_address << endl;
        const uint32_t symbol_section_size = get_symbols_size(symbol_section_address);
        writer << "Symbol section size: 0x" << hex << symbol_section_size << endl;

        const uint32_t not_aligned_code_address = symbol_section_address + symbol_section_size;
        writer << "Not aligned code address: 0x" << not_aligned_code_address << endl;
        const uint32_t code_address = not_aligned_code_address % 16 ? not_aligned_code_address + (16 - (not_aligned_code_address % 16)) : not_aligned_code_address;
        writer << "Code: 0x" << code_address << endl;
        const uint32_t rodata_address = code_address + header.code_size();
        writer << "Rodata: 0x" << rodata_address << endl;
        const uint32_t size_of_lot = get_size_of_lot(header, relocation_section_address);
        writer << "Size of LOT: " << dec << size_of_lot << endl;

        const auto* main = find_symbol(symbol_section_address, "main");
        modules_.emplace_back(header);
        LoadedModule& loaded_module = modules_.back();
        writer << "Module address: 0x" << hex << reinterpret_cast<const uint32_t>(&modules_.front()) << hex << ", 0x" << reinterpret_cast<const uint32_t>(&modules_.back()) << endl;
        Module& module = loaded_module.get_module();
        ModuleData& module_data = module.get_module_data();

        if (mode & LoadingModeCopyText)
        {
            module.allocate_text();
            std::memcpy(module.get_text().data(), reinterpret_cast<const uint8_t*>(code_address), header.code_size());
        }
        else
        {
            module.set_text(gsl::make_span(reinterpret_cast<uint8_t*>(code_address), header.code_size()));
        }

        if (mode & LoadingModeCopyRodata)
        {
            module.allocate_rodata();
            std::memcpy(module.get_rodata().data(), reinterpret_cast<uint8_t*>(rodata_address), header.rodata_size());
        }
        else
        {
            module.set_rodata(gsl::make_span(reinterpret_cast<uint8_t*>(rodata_address), header.rodata_size()));
        }

        module.allocate_data();
        std::memset(module.get_data().data(), 0, header.data_size());

        if (main)
        {
            const std::size_t main_function_address = reinterpret_cast<uint32_t>(module.get_text().data()) + main->offset() - 1;
            loaded_module.set_start_address(main_function_address);
        }

        process_relocations(relocation_section_address, environment, loaded_module);

        return &loaded_module;
    }

    const Symbol* find_symbol(const uint32_t address, const std::string_view& symbol_name)
    {
        const uint32_t number_of_symbols = *reinterpret_cast<uint32_t*>(address);
        const Symbol* symbol = reinterpret_cast<const Symbol*>(address + 4);
        for (uint32_t i = 0; i < number_of_symbols; ++i)
        {
            if (symbol->name() == symbol_name)
            {
                return symbol;
            }
            symbol = &symbol->next();
        }
        return nullptr;
    }

    uint32_t get_lot_for_module_at(uint32_t address)
    {
        auto* backm = &modules_.front();
        writer << "Loading LOT address: 0x" << hex << reinterpret_cast<const uint32_t>(backm) << endl;

        for (const auto& loaded_module : modules_)
        {
            writer << "Module address: 0x" << hex << reinterpret_cast<const uint32_t>(&loaded_module) << endl;
            const Module& module = loaded_module.get_module();
            const uint32_t text_address = reinterpret_cast<uint32_t>(module.get_text().data());
            writer << module.get_header().name() << " -> Address: 0x" << hex << address << ", text: 0x" << text_address << ", size: 0x" << module.get_header().code_size() << endl;
            if (address >= text_address && address < text_address + module.get_header().code_size())
            {
                return reinterpret_cast<uint32_t>(module.get_lot().data());
            }
        }

        return 0x0; 
    }


private:

    uint32_t get_size_of_lot(const ModuleHeader& header, uint32_t address)
    {
        uint32_t size_of_lot = 0;
        const Relocation* relocation = reinterpret_cast<const Relocation*>(address);

        for (int i = 0; i < header.number_of_relocations(); ++i)
        {
            const Symbol& symbol = relocation->symbol();

            if (symbol.visibility() == SymbolVisibility::internal || symbol.visibility() == SymbolVisibility::exported)
            {
                if (symbol.section() == Section::rodata)
                {
                    ++size_of_lot;
                }
            }
            else if (symbol.visibility() == SymbolVisibility::external)
            {
                ++size_of_lot;
            }
            relocation = &relocation->next();
        }
        return size_of_lot;
    }

    template <typename Environment>
    void process_relocations(uint32_t address, const Environment& env, LoadedModule& loaded_module)
    {
        Module& module = loaded_module.get_module();
        const ModuleHeader& header = module.get_header();
        auto& lot = module.get_lot();
        const uint32_t rodata = reinterpret_cast<const uint32_t>(module.get_rodata().data());

        lot.reserve(get_size_of_lot(header, address));

        for (int i = 0; i < header.number_of_relocations(); ++i)
        {
            const Relocation& relocation = *reinterpret_cast<const Relocation*>(address);
            address += relocation.size();
            const Symbol& symbol = relocation.symbol();

            if (symbol.visibility() == SymbolVisibility::internal || symbol.visibility() == SymbolVisibility::exported)
            {
                if (symbol.section() == Section::rodata)
                {
                    lot[i] = rodata;
                }
                else if (symbol.section() == Section::code)
                {
                }
            }
            else if (symbol.visibility() == SymbolVisibility::external)
            {
                writer << "Searching symbol address for: " << symbol.name() << endl;
                const auto* env_symbol = env.find_symbol(symbol.name());
                if (env_symbol)
                {
                    writer << "Symbol found in env at address 0x" << hex << env_symbol->address();
                    lot[i] = env_symbol->address();
                }
            }
        }
    }

    uint32_t get_relocations_size(const ModuleHeader& header, const uint32_t address)
    {
        uint32_t size = 0;
        const Relocation* relocation = reinterpret_cast<const Relocation*>(address);

        for (int i = 0; i < header.number_of_relocations(); ++i)
        {
            writer << "Relocation: " << relocation->symbol().name() << ", index: " << dec << relocation->index() << endl;
            size += relocation->size();
            relocation = &relocation->next();
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
            symbol = &symbol->next();
        }
        return size;
    }


private:
    eul::container::static_vector<LoadedModule, 10> modules_;
};

} // namespace dl
} // namespace msos

