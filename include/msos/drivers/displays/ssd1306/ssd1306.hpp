#pragma once

#include <cstdint>

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

constexpr uint8_t SSD1306_DEFAULT_ADDRESS = 0x3C;

template <typename I2CInterface>
class SSD1306_I2C
{
public:
    SSD1306_I2C(const uint8_t address = SSD1306_DEFAULT_ADDRESS) : address_(address)
    {
        I2CInterface::init();
        sendCommand(SSD1306_SET_DISPLAY_OFF);
        sendCommand(SSD1306_SET_DISPLAY_CLOCK_DIVIDE_RATIO);
        sendCommand(0xf0);
        sendCommand(SSD1306_SET_MULTIPLEX_RATIO);
        sendCommand(0x7E);
        sendCommand(SSD1306_SET_DISPLAY_OFFSET);
        sendCommand(0x00);
        sendCommand(SSD1306_SET_DISPLAY_START_LINE);
        sendCommand(SSD1306_CHARGE_PUMP);
        sendCommand(0x14);

        sendCommand(SSD1306_SET_MEMORY_ADDRESSING_MODE);
        sendCommand(0x00);

        sendCommand(SSD1306_SET_SEGMENT_REMAP_1);
        sendCommand(SSD1306_SET_COM_OUTPUT_SCAN_INCREMENTAL);
        sendCommand(SSD1306_SET_COM_PINS_HARDWARE_CONFIGURATION);
        sendCommand(0x12);
        sendCommand(SSD1306_SET_CONTRAST_CONTROL_FOR_BANK0);
        sendCommand(0xCF);
        sendCommand(SSD1306_SET_PRE_CHARGE_PERIOD);
        sendCommand(0xF1);
        sendCommand(SSD1306_SET_VCOMH_DESELECT_LEVEL);
        sendCommand(0x40);
        sendCommand(SSD1306_ENTIRE_DISPLAY_ON_RESUME);
        sendCommand(SSD1306_SET_DISPLAY_ON);
    }

    constexpr uint32_t getHeight() const
    {
        return 64;
    }

    constexpr uint32_t getWidth() const
    {
        return 128;
    }

    void setPixel(uint16_t x, uint16_t y)
    {

    }

    void display(const gsl::span<uint8_t>& buffer)
    {
        setHome();

        for (uint8_t packet = 0; packet < buffer.size()/16; packet++)
        {
            I2CInterface::start(address_);
            I2CInterface::write(SSD1306_SET_DISPLAY_START_LINE);
            for (uint8_t packet_byte = 0; packet_byte < 16; ++packet_byte)
            {
                I2CInterface::write(buffer[packet*16+packet_byte]);
            }
            I2CInterface::stop();
        }
    }

    void clear()
    {

    }
private:
    void setHome() const
    {
        sendCommand(SSD1306_SET_COLUMN_ADDRESS);
        sendCommand(0x00);
        sendCommand(0x7F);

        sendCommand(SSD1306_SET_PAGE_ADDRESS);
        sendCommand(0x00);
        sendCommand(0x07);
    }

    void sendCommand(const uint8_t command) const
    {
        I2CInterface::start(address_);
        I2CInterface::write(0x00);
        I2CInterface::write(command);
        I2CInterface::stop();
    }

    const uint8_t address_;
};

} // namespace displays
} // namespace drivers
} // namespace msos
