#pragma once

#include <string_view>
#include <list>
#include <vector>
#include <optional>

#include <gsl/span>

#include "msos/dynamic_linker/module.hpp"
#include "msos/dynamic_linker/symbol.hpp"
#include "msos/dynamic_linker/relocation.hpp"
#include "msos/usart_printer.hpp"

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

class LoadedModuleList
{

};

constexpr int LoadingModeCopyData = 0x01;
constexpr int LoadingModeCopyText = 0x02;
constexpr int LoadingModeCopyRodata = 0x02;

class ModuleData
{
public:
    using SectionMemory = gsl::span<const uint8_t>;
    using LOTMemory = gsl::span<uint32_t>;
    ~ModuleData()
    {
        if (rodata_)

    }
    SectionMemory get_data_memory() const
    SectionMemory get_rodata_memory() const
    SectionMemory get_text_memory() const
    LOTMemory get_lot() const

private:
    Module* module_;
    std::unique_ptr<uint8_t[]> rodata_;
    std::unique_ptr<uint8_t[]> data_;
    std::unique_ptr<uint8_t[]> text_;
    std::vector<uint32_t> lot_;
};

class LoadedModule
{
public:
    LoadedModule(bool is_executable, const Module* module, const std::size_t main_address, std::unique_ptr<IModuleData>&& data)
        : is_executable_(is_executable)
        , module_(module)
        , main_address_(main_address)
        , data_(std::move(data))
    {
    }

    const Module* module() const
    {
        return module_;
    }

    const int execute(int argc, char * argv[]) const
    {
        // if (is_executable_)
        // {
        //     return call_external(main_address_, argc, argv);
        // }
        return -1;
    }

    const int execute() const
    {
        if (is_executable_)
        {
            return call_external(main_address_);
        }
        return -1;
    }

    const std::vector<uint32_t>& get_lot() const
    {
        return lot_table_;
    }

    std::vector<uint32_t>& get_lot()
    {
        return data_.lot_table_;
    }

    bool operator==(const LoadedModule& m) const
    {
        return m.module_ == module_;
    }
private:
    bool is_executable_;
    const Module* module_;
    const std::size_t main_address_;
    std::unique_ptr<IModuleData> data_;
};


class ModuleData : public ModuleDataMemory, ModuleLOTMemory, ModuleRoDataBridge, ModuleTextBridge
{
public:
    ModuleData(const SectionMemory& rodata, const SectionMemory& text)
        : ModuleRoDataBridge(rodata), ModuleTextBridge(text)
    {}
};

class ModuleDataWithTextMemory : public ModuleDataMemory, ModuleLOTMemory, ModuleRoDataBridge, ModuleTextMemory
{
public:
    ModuleData(const SectionMemory& rodata)
        : ModuleRoDataBridge(rodata)
    {}
};

enum class LoadingStatus
{
    ModuleCookieValidationFailed,
    Ok
};


class DynamicLinker
{
public:
    void unload_module(const LoadedModule* module)
    {
        auto module_it = std::find(loaded_modules_.begin(), loaded_modules_.end(), *module);
        if (module_it != loaded_modules_.end())
        {
            loaded_modules_.erase(module_it);
        }
    }

    template <typename Environment>
    LoadedModule* load_module(const uint32_t module_address, const int mode, const Environment& environment)
    {
        writer << "Loading module from: 0x" << hex << module_address << endl;
        const Module* module = reinterpret_cast<const Module*>(module_address);

        if (module->cookie() != "MSDL")
        {
            return nullptr;//;LoadingStatus::ModuleCookieValidationFailed;
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

        // std::vector<uint32_t> lot = process_relocations(module, relocation_section_address, code_address, rodata_address, environment);
        // writer << "Address of LOT: 0x" << hex << reinterpret_cast<uint32_t>(lot.data()) << endl;

        uint32_t code_address_in_memory = 0x0;
        uint32_t rodata_address_in_memory = 0x0;

        std::unique_ptr<IModuleData> data;

        if (mode & LoadingModeCopyText)
        {

        }

        if (mode & LoadingModeCopyText)
        {

        }

        if (mode & LoadingModeCopyText)
        {
            uint8_t* text = new uint8_t[module->code_size()];
            std::memcpy(text, reinterpret_cast<const uint8_t*>(code_address), module->code_size());
            code_address_in_memory = reinterpret_cast<uint32_t>(text);
        }

        if (mode & LoadingModeCopyRodata)
        {
            uint8_t* rodata = new uint8_t[module->rodata_size()];
            std::memcpy(rodata, reinterpret_cast<const uint8_t*>(rodata_address), module->rodata_size());
            rodata_address_in_memory = reinterpret_cast<uint32_t>(rodata);
        }


        uint8_t* data = new uint8_t[module->rodata_size()];
        std::memcpy(rodata, reinterpret_cast<const uint8_t*>(rodata_address), module->rodata_size());
        rodata_address_in_memory = reinterpret_cast<uint32_t>(rodata);

        const auto* main = find_symbol(symbol_section_address, "main");
        const std::size_t main_function_address = code_address_in_memory + main->offset() - 1;

        if (main != nullptr)
        {
            loaded_modules_.emplace_back(true, module, main_function_address);
        }
        else
        {
            loaded_modules_.emplace_back(false, module, main_function_address);
        }

        process_relocations(module, relocation_section_address, code_address_in_memory, rodata_address_in_memory, environment, loaded_modules_.back());

        return &loaded_modules_.back();
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
    void process_relocations(const Module* module, uint32_t address, uint32_t code, uint32_t rodata, const Environment& env, LoadedModule& loaded_module)
    {
        loaded_module.get_lot().reserve(get_size_of_lot(module, address));

        for (int i = 0; i < module->number_of_relocations(); ++i)
        {
            const Relocation* relocation = reinterpret_cast<const Relocation*>(address);
            address += relocation->size();
            const Symbol* symbol = relocation->symbol();

            if (symbol->visibility() == SymbolVisibility::internal || symbol->visibility() == SymbolVisibility::exported)
            {
                if (symbol->section() == Section::rodata)
                {
                    loaded_module.get_lot()[i] = rodata;
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
                    loaded_module.get_lot()[i] = env_symbol->address();
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
    std::list<LoadedModule> loaded_modules_;
};

} // namespace dl
} // namespace msos

