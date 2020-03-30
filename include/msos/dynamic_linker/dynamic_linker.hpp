// This file is part of MSOS project. This is simple OS for embedded development devices.
// Copyright (C) 2019 Mateusz Stadnik
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

#pragma once

#include <string_view>
#include <cstring>
#include <optional>
#include <eul/container/static_vector.hpp>

#include <gsl/span>

#include "msos/dynamic_linker/module.hpp"
#include "msos/dynamic_linker/module_header.hpp"
#include "msos/dynamic_linker/symbol.hpp"
#include "msos/dynamic_linker/relocation.hpp"
#include "msos/dynamic_linker/module_data.hpp"
#include "msos/dynamic_linker/environment.hpp"
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

static UsartWriter writer;

class ErrorCode
{
public:
    virtual ~ErrorCode() = default;
    virtual bool operator bool() const;
    virtual std::string_view message() const;
};

class DynamicLinker
{
public:
    DynamicLinker() : modules_{}
    {
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
    const LoadedModule* load_module(const std::size_t* module_address, const int mode, const Environment& environment)
    {
        return load_module(module_address, mode, reinterpret_cast<const SymbolEntry*>(environment.data().data()), environment.data().size());
    }

    const LoadedModule* load_module(const std::size_t* module_address, const int mode, const SymbolEntry* entries, int number_of_entries)
    {
        const ModuleHeader& header = *reinterpret_cast<const ModuleHeader*>(module_address);

        if (header.cookie() != "MSDL")
        {
            return nullptr;//;LoadingStatus::ModuleCookieValidationFailed;
        }

        const std::size_t relocation_section_address = reinterpret_cast<const std::size_t>(module_address) + header.size();
        const uint32_t relocation_section_size = get_relocations_size(header, relocation_section_address);
        for (int i = 0; i < header.number_of_relocations(); ++i)
        {
            const Relocation& relocation = *(reinterpret_cast<const Relocation*>(relocation_section_address) + i);
        }
        const std::size_t symbol_section_address = relocation_section_address + relocation_section_size;
        const uint32_t symbol_section_size = get_symbols_size(symbol_section_address, header.number_of_external_symbols() + header.number_of_exported_symbols());

        const std::size_t not_aligned_code_address = symbol_section_address + symbol_section_size;
        const std::size_t code_address = not_aligned_code_address % 16 ? not_aligned_code_address + (16 - (not_aligned_code_address % 16)) : not_aligned_code_address;
        const std::size_t data_address = code_address + header.code_size();
        const uint32_t size_of_lot = get_size_of_lot(header);

        const auto* main = find_symbol(symbol_section_address, header.number_of_external_symbols() + header.number_of_exported_symbols(), "main");
        modules_.emplace_back(header);
        LoadedModule& loaded_module = modules_.back();
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

        module.allocate_data();
        const uint8_t* dd = reinterpret_cast<const uint8_t*>(data_address);

        std::memcpy(module.get_data().data(), reinterpret_cast<const uint8_t*>(data_address), header.data_size());
        std::memset(module.get_data().data() + header.data_size(), 0, header.bss_size());

        if (main)
        {
            const std::size_t main_function_address = (reinterpret_cast<std::size_t>(module.get_text().data()) + main->offset()) & (~0x01);
            loaded_module.set_start_address(main_function_address);
        }

        if (!allocate_lot(loaded_module)) return nullptr;
        if (!process_exported_relocations(relocation_section_address, loaded_module)) return nullptr;

        const std::size_t external_relocations_address = relocation_section_address + header.number_of_exported_relocations() * sizeof(Relocation);
        if (!process_external_relocations(external_relocations_address, entries, number_of_entries, loaded_module)) return nullptr;

        const std::size_t local_relocations_address = external_relocations_address + sizeof(Relocation) * header.number_of_external_relocations();
        if (!process_local_relocations(local_relocations_address, loaded_module))
        {
            writer << "Local relocations failed" << endl;
            return nullptr;
        }

        const std::size_t data_relocations_address = local_relocations_address + sizeof(Relocation) * header.number_of_local_relocations();
        if (!process_data_relocations(local_relocations_address, loaded_module))
        {
            writer << "Data relocations failed" << endl;
            return nullptr;
        }

        return &loaded_module;
    }


    const Symbol* find_symbol(const std::size_t address, const uint32_t number_of_symbols, const std::string_view& symbol_name)
    {
        const Symbol* symbol = reinterpret_cast<const Symbol*>(address);
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

    uint32_t get_lot_for_module_at(std::size_t address)
    {
        auto* backm = &modules_.front();

        for (const auto& loaded_module : modules_)
        {
            const Module& module = loaded_module.get_module();
            const uint32_t text_address = reinterpret_cast<std::size_t>(module.get_text().data());
            if (address >= text_address && address < text_address + module.get_header().code_size())
            {
                return reinterpret_cast<std::size_t>(module.get_lot().get());
            }
        }

        return 0x0;
    }


private:

    uint32_t get_size_of_lot(const ModuleHeader& header)
    {
        return header.number_of_external_symbols() + header.number_of_local_relocations();
    }

    bool allocate_lot(LoadedModule& loaded_module)
    {
        Module& module = loaded_module.get_module();
        const ModuleHeader& header = module.get_header();
        auto& lot = module.get_lot();
        lot.reset(new uint32_t[header.number_of_external_relocations() + header.number_of_local_relocations()]);
        return lot != nullptr;
    }

    bool process_exported_relocations(std::size_t exported_relocations_address, LoadedModule& loaded_module)
    {
        Module& module = loaded_module.get_module();
        const ModuleHeader& header = module.get_header();
        auto& lot = module.get_lot();

        for (int i = 0; i < header.number_of_exported_relocations(); ++i)
        {
            const Relocation& relocation = *reinterpret_cast<const Relocation*>(exported_relocations_address);
            exported_relocations_address += relocation.size();
            const Symbol& symbol = relocation.symbol();
            if (symbol.section() == Section::code)
            {
                const std::size_t relocated = reinterpret_cast<const std::size_t>(module.get_text().data()) + symbol.offset();
                lot[relocation.index()] = relocated;
            }
            else if (symbol.section() == Section::data)
            {
                const std::size_t relocated = reinterpret_cast<const std::size_t>(module.get_data().data()) + symbol.offset();
                lot[relocation.index()] = relocated;
            }
            else
            {
                return false;
            }
        }
        return true;
    }

    bool process_local_relocations(std::size_t local_relocations_address, LoadedModule& loaded_module)
    {
        Module& module = loaded_module.get_module();
        const ModuleHeader& header = module.get_header();
        auto& lot = module.get_lot();

        for (int i = 0; i < header.number_of_local_relocations(); ++i)
        {
            const Relocation& relocation = *reinterpret_cast<const Relocation*>(local_relocations_address);
            local_relocations_address += relocation.size();
            uint32_t lot_index = relocation.index() >> 1;
            uint16_t section_value = relocation.index() & 0x01;
            Section section = static_cast<Section>(section_value);
            if (section == Section::code)
            {
                const uint32_t relocated = reinterpret_cast<const std::size_t>(module.get_text().data()) + relocation.offset();
                lot[lot_index] = relocated;
            }
            else if (section == Section::data)
            {
                const uint32_t relocated = reinterpret_cast<const std::size_t>(module.get_data().data()) + relocation.offset();
                lot[lot_index] = relocated;
            }
            else
            {
                return false;
            }
        }
        return true;

    }


    template <typename Environment>
    bool process_external_relocations(std::size_t external_relocations_address, const Environment& env, LoadedModule& loaded_module)
    {
        Module& module = loaded_module.get_module();
        const ModuleHeader& header = module.get_header();
        auto& lot = module.get_lot();

        for (int i = 0; i < header.number_of_external_relocations(); ++i)
        {
            const Relocation& relocation = *reinterpret_cast<const Relocation*>(external_relocations_address);

            external_relocations_address += relocation.size();
            const Symbol& symbol = relocation.symbol();
            const auto* env_symbol = env.find_symbol(symbol.name());
            if (env_symbol)
            {
                lot[relocation.index()] = env_symbol->address();
            }
            else
            {
                return false;
            }
        }
        return true;
    }

    const SymbolEntry* find_symbol(const SymbolEntry* entries, int number_of_entries, std::string_view symbol)
    {
        for (int i = 0; i < number_of_entries; ++i)
        {
            if (std::string_view(entries[i].name) == symbol)
            {
                return &entries[i];
            }
        }
        return nullptr;
    }

    bool process_external_relocations(std::size_t external_relocations_address, const SymbolEntry* entries, int number_of_entries, LoadedModule& loaded_module)
    {
        Module& module = loaded_module.get_module();
        const ModuleHeader& header = module.get_header();
        auto& lot = module.get_lot();

        for (int i = 0; i < header.number_of_external_relocations(); ++i)
        {
            const Relocation& relocation = *reinterpret_cast<const Relocation*>(external_relocations_address);

            external_relocations_address += relocation.size();
            const Symbol& symbol = relocation.symbol();
            const auto* env_symbol = find_symbol(entries, number_of_entries, symbol.name());
            if (env_symbol)
            {
                lot[relocation.index()] = env_symbol->address;
            }
            else
            {
                writer << "Can't find symbol: " << symbol.name() << endl;
                return false;
            }
        }
        return true;
    }


    template <typename Environment>
    bool process_relocations(uint32_t address, const Environment& env, LoadedModule& loaded_module)
    {
       //  Module& module = loaded_module.get_module();
       //  const ModuleHeader& header = module.get_header();
       //  auto& lot = module.get_lot();

       //  lot.reset(new uint32_t[header.number_of_relocations()]);

       //  for (int i = 0; i < header.number_of_relocations(); ++i)
       //  {
       //      const Relocation& relocation = *reinterpret_cast<const Relocation*>(address);
       //      address += relocation.size();
       //      const Symbol& symbol = relocation.symbol();

       //      if (symbol.visibility() == SymbolVisibility::internal || symbol.visibility() == SymbolVisibility::exported)
       //      {
       //          if (symbol.section() == Section::code)
       //          {
       //              uint32_t relocated = reinterpret_cast<uint32_t>(loaded_module.get_module().get_text().data()) + symbol.offset();
       //              lot[i] = relocated;
       //              writer_ << "Symbol " << symbol.name() << ", relocated in table[0x" << hex << i << "], to 0x" << lot[i] << endl;
       //          }
       //          if (symbol.section() == Section::data)
       //          {
       //              uint32_t relocated = reinterpret_cast<uint32_t>(loaded_module.get_module().get_data().data()) + symbol.offset();
       //              lot[i] = relocated;
       //              writer_ << "Symbol " << symbol.name() << ", relocated in table[0x" << hex << i << "], to 0x" << lot[i] << endl;
       //          }
       //      }
       //      else if (symbol.visibility() == SymbolVisibility::external)
       //      {
       //          writer_ << "Searching symbol address for: " << symbol.name() << endl;
       //          const auto* env_symbol = env.find_symbol(symbol.name());
       //          if (env_symbol)
       //          {
       //              writer_ << "Symbol found in env at address 0x" << hex << env_symbol->address();
       //              lot[i] = env_symbol->address();
       //          }
       //          else
       //          {
       //              writer_ << "Address for symbol: " << symbol.name() << " not found!" << endl;
       //              return false;
       //          }
       //      }
       //  }

       //  for (int i = header.number_of_relocations(); i < header.total_relocations(); ++i)
       //  {
       //      writer_ << "Data relocation:" << endl;
       //      const Relocation& relocation = *reinterpret_cast<const Relocation*>(address);
       //      address += relocation.size();
       //      const Symbol& symbol = relocation.symbol();

       //      uint32_t *to_relocate = reinterpret_cast<uint32_t*>(reinterpret_cast<uint32_t>(module.get_data().data())) + relocation.index();
       //      if (symbol.section() == Section::code)
       //      {
       //          uint32_t relocated = reinterpret_cast<uint32_t>(module.get_text().data()) + symbol.offset();
       //          *to_relocate = relocated;
       //          writer_ << "Symbol with offest: 0x" << hex << symbol.offset() << symbol.name() << ", relocated on 0x" << hex << reinterpret_cast<uint32_t>(to_relocate) << ", index: " << dec << i << ", to 0x" << hex << relocated << endl;
       //      }

       //      else if (symbol.section() == Section::data)
       //      {
       //          uint32_t relocated = reinterpret_cast<uint32_t>(module.get_data().data()) + (symbol.offset() & ~(0x01));
       //          *to_relocate = relocated;
       //          writer_ << "Symbol " << symbol.name() << ", relocated on 0x" << hex << reinterpret_cast<uint32_t>(to_relocate) << ", index: " << dec << i << ", to 0x" << hex << relocated << endl;
       //      }

       //  }
        return true;
    }

    bool process_data_relocations(std::size_t data_relocations_address, LoadedModule& loaded_module)
    {
        Module& module = loaded_module.get_module();
        const ModuleHeader& header = module.get_header();
        for (int i = 0; i < header.number_of_data_relocations(); ++i)
        {
            const Relocation& relocation = *reinterpret_cast<const Relocation*>(data_relocations_address);
            data_relocations_address += relocation.size();

            std::size_t *to_relocate = reinterpret_cast<std::size_t*>(reinterpret_cast<std::size_t>(module.get_data().data())) + relocation.index();

            std::size_t relocated = reinterpret_cast<std::size_t>(module.get_data().data()) + relocation.offset();
            *to_relocate = relocated;

        }
        return true;
    }

    uint32_t get_relocations_size(const ModuleHeader& header, const uint32_t address)
    {
        return sizeof(Relocation) * header.number_of_relocations();
    }

    uint32_t get_symbols_size(const std::size_t address, const uint32_t number_of_symbols)
    {
        uint32_t size = 0;

        const Symbol* symbol = reinterpret_cast<const Symbol*>(address);
        for (uint32_t i = 0; i < number_of_symbols; ++i)
        {
            size += symbol->size();
            symbol = &symbol->next();
        }
        return size;
    }


private:
    eul::container::static_vector<LoadedModule, 10> modules_;
    UsartWriter writer_;
};

} // namespace dl
} // namespace msos

