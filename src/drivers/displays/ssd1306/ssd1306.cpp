// This file is part of MSOS project. This is simple OS for embedded development devices.
// Copyright (C) 2020 Mateusz Stadnik
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

#include "msos/drivers/displays/ssd1306/ssd1306.hpp"

#include <misc.h>

#include <cstring>

#include <hal/time/sleep.hpp>

#include "msos/usart_printer.hpp"

namespace msos
{
namespace drivers
{
namespace displays
{
constexpr uint8_t SSD1306_SET_LOWER_COLUMN_START_ADDRESS = 0x00;
constexpr uint8_t SSD1306_SET_HIGHER_COLUMN_START_ADDRESS = 0x10;
constexpr uint8_t SSD1306_SET_COLUMN_ADDRESS = 0x21;
constexpr uint8_t SSD1306_SET_PAGE_ADDRESS = 0x22;
constexpr uint8_t SSD1306_SET_MEMORY_ADDRESSING_MODE = 0x20;
constexpr uint8_t SSD1306_SET_DISPLAY_START_LINE = 0x40;
constexpr uint8_t SSD1306_SET_CONTRAST_CONTROL_FOR_BANK0 = 0x81;
constexpr uint8_t SSD1306_SET_SEGMENT_REMAP_1 = 0xA0;
constexpr uint8_t SSD1306_SET_SEGMENT_REMAP_2 = 0xA1;
constexpr uint8_t SSD1306_ENTIRE_DISPLAY_ON_RESUME = 0xA4;
constexpr uint8_t SSD1306_ENTIRE_DISPLAY_ON = 0xA5;
constexpr uint8_t SSD1306_SET_NORMAL_DISPLAY = 0xA6;
constexpr uint8_t SSD1306_SET_INVERSE_DISPLAY = 0xA7;
constexpr uint8_t SSD1306_SET_MULTIPLEX_RATIO = 0xA8;
constexpr uint8_t SSD1306_SET_DISPLAY_ON = 0xAF;
constexpr uint8_t SSD1306_SET_DISPLAY_OFF = 0xAE;
constexpr uint8_t SSD1306_SET_PAGE_START_ADDRESS = 0xB0;
constexpr uint8_t SSD1306_SET_COM_OUTPUT_SCAN_INCREMENTAL = 0xC0;
constexpr uint8_t SSD1306_SET_COM_OUTPUT_SCAN_DECREMENTAL = 0xC8;
constexpr uint8_t SSD1306_SET_DISPLAY_OFFSET = 0xD3;
constexpr uint8_t SSD1306_SET_DISPLAY_CLOCK_DIVIDE_RATIO = 0xD5;
constexpr uint8_t SSD1306_SET_PRE_CHARGE_PERIOD = 0xD9;
constexpr uint8_t SSD1306_SET_COM_PINS_HARDWARE_CONFIGURATION = 0xDA;
constexpr uint8_t SSD1306_SET_VCOMH_DESELECT_LEVEL = 0xDB;
constexpr uint8_t SSD1306_NOP = 0xE3;
constexpr uint8_t SSD1306_CHARGE_PUMP = 0x8D;

static UsartWriter writer;

volatile bool waiting_for_write_ = false;

hal::interfaces::I2C* i2cc;

SSD1306_I2C::SSD1306_I2C(hal::interfaces::I2C& i2c, const uint8_t address)
    : i2c_(i2c)
    , address_(address)
    , buffer_(new uint8_t[1024])
{
    std::memset(buffer_, 0 , 1024);
    i2cc = &i2c;
}

void SSD1306_I2C::load()
{
    hal::time::sleep(std::chrono::milliseconds(100));
    i2c_.init();

    sendCommand(SSD1306_SET_DISPLAY_OFF);
    sendCommand(SSD1306_SET_MULTIPLEX_RATIO);
    sendCommand(0x3f);
    sendCommand(SSD1306_SET_DISPLAY_OFFSET);
    sendCommand(0x00);
    sendCommand(SSD1306_SET_DISPLAY_START_LINE);
    sendCommand(SSD1306_SET_SEGMENT_REMAP_2);
    sendCommand(SSD1306_SET_COM_OUTPUT_SCAN_DECREMENTAL);
    sendCommand(SSD1306_SET_COM_PINS_HARDWARE_CONFIGURATION);
    sendCommand(0x12);
    sendCommand(SSD1306_SET_CONTRAST_CONTROL_FOR_BANK0);
    sendCommand(0xFF);
    sendCommand(SSD1306_ENTIRE_DISPLAY_ON_RESUME);

    sendCommand(SSD1306_SET_NORMAL_DISPLAY);

    sendCommand(SSD1306_SET_DISPLAY_CLOCK_DIVIDE_RATIO);
    sendCommand(0xF0);

    sendCommand(SSD1306_CHARGE_PUMP);
    sendCommand(0x14);

    sendCommand(SSD1306_SET_MEMORY_ADDRESSING_MODE);
    sendCommand(0x00);


    sendCommand(SSD1306_SET_PRE_CHARGE_PERIOD);
    sendCommand(0x14);
    // sendCommand(SSD1306_SET_VCOMH_DESELECT_LEVEL);
    // sendCommand(0x20);
    sendCommand(SSD1306_SET_DISPLAY_ON);

    clear();
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
    DMA_DeInit(DMA1_Channel6);
    dma_config_.DMA_PeripheralBaseAddr = (uint32_t)&(I2C1->DR);
    dma_config_.DMA_DIR = DMA_DIR_PeripheralDST;
    dma_config_.DMA_MemoryBaseAddr = (uint32_t)buffer_;
    dma_config_.DMA_BufferSize = 1024;
    dma_config_.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    dma_config_.DMA_MemoryInc = DMA_MemoryInc_Enable;
    dma_config_.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    dma_config_.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
    dma_config_.DMA_Mode = DMA_Mode_Normal;
    dma_config_.DMA_Priority = DMA_Priority_High;
    dma_config_.DMA_M2M = DMA_M2M_Disable;
    DMA_Init(DMA1_Channel6, &dma_config_);
    NVIC_InitTypeDef nvicStructure;
    nvicStructure.NVIC_IRQChannel = DMA1_Channel6_IRQn;
    nvicStructure.NVIC_IRQChannelPreemptionPriority = 0;
    nvicStructure.NVIC_IRQChannelSubPriority = 1;
    nvicStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&nvicStructure);
}

bool SSD1306_I2C::busy()
{
    return waiting_for_write_;
}

void SSD1306_I2C::unload()
{
}

void SSD1306_I2C::clear()
{
    while (waiting_for_write_);
    setHome();
    for (int x = 0; x < 128; x++)
    {
        for (int page = 0; page < 8; page++)
        {
            i2c_.start(address_);
            i2c_.write(SSD1306_SET_DISPLAY_START_LINE);
            i2c_.write(0);
            i2c_.stop();
        }
    }
}

void SSD1306_I2C::display(const gsl::span<uint8_t>& buffer)
{
    while (waiting_for_write_);

    setHome();

    for (uint8_t packet = 0; packet < buffer.size()/16; packet++)
    {
        i2c_.start(address_);
        i2c_.write(SSD1306_SET_DISPLAY_START_LINE);
        for (uint8_t packet_byte = 0; packet_byte < 16; ++packet_byte)
        {
            i2c_.write(buffer[packet*16+packet_byte]);
        }
        i2c_.stop();
    }
}

void SSD1306_I2C::write(const uint8_t byte)
{
    while (waiting_for_write_);

    i2c_.start(address_);
    i2c_.write(SSD1306_SET_DISPLAY_START_LINE);
    i2c_.write(byte);
    i2c_.stop();
}

void SSD1306_I2C::write(gsl::span<const char> buffer)
{
    std::memcpy(buffer_, buffer.data(), buffer.size());

    setHome();

    while (waiting_for_write_);
    waiting_for_write_ = true;
    i2c_.start(address_);
    i2c_.write(SSD1306_SET_DISPLAY_START_LINE);
    DMA1_Channel6->CNDTR = 1024;
    I2C_DMACmd(I2C1, ENABLE);
    DMA_Cmd(DMA1_Channel6, ENABLE);
    DMA_ClearITPendingBit(DMA1_IT_TC6);
    DMA_ITConfig(DMA1_Channel6, DMA_IT_TC, ENABLE);
}

std::unique_ptr<fs::IFile> SSD1306_I2C::file(std::string_view path)
{
    return std::make_unique<fs::Ssd1306File>(*this, path);
}

void SSD1306_I2C::setHome()
{
    while (waiting_for_write_);

    sendCommand(SSD1306_SET_COLUMN_ADDRESS);
    sendCommand(0x00);
    sendCommand(0x7F);

    sendCommand(SSD1306_SET_PAGE_ADDRESS);
    sendCommand(0x00);
    sendCommand(0x07);
}

void SSD1306_I2C::sendCommand(const uint8_t command)
{
    while (waiting_for_write_);

    i2c_.start(address_);
    i2c_.write(0x00);
    i2c_.write(command);
    i2c_.stop();
}

} // namespace displays
} // namespace drivers
} // namespace msos


extern "C" void DMA1_Channel6_IRQHandler()
{
    DMA_ClearITPendingBit(DMA1_IT_TC6);
    DMA_Cmd(DMA1_Channel6, DISABLE);
    DMA_ITConfig(DMA1_Channel6, DMA_IT_TC, DISABLE);
    msos::drivers::displays::i2cc->stop();
    msos::drivers::displays::waiting_for_write_ = false;
}
