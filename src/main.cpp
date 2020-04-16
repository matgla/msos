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

#include <cstdint>
#include <filesystem>

#include <board.hpp>
#include <hal/core/core.hpp>
#include <hal/time/sleep.hpp>
#include <hal/time/time.hpp>

#include "msos/fs/romfs.hpp"
#include "msos/drivers/storage/ram_block_device.hpp"
#include "msos/drivers/character/usart/usart_driver.hpp"
#include "msos/drivers/device_fs.hpp"
#include "msos/fs/ramfs.hpp"
#include "msos/fs/vfs.hpp"
#include "msos/fs/mount_points.hpp"
#include "msos/kernel/process/spawn.hpp"
#include "msos/kernel/process/scheduler.hpp"
#include "msos/syscalls/syscalls.hpp"
#include "msos/usart_printer.hpp"
#include "msos/libc/printf.hpp"
#include "msos/apps/app_registry.hpp"
#include "msos/drivers/displays/ssd1306/ssd1306.hpp"
#include "msgui/Factory.hpp"
#include "msgui/policies/chunk/SSD1308ChunkPolicy.hpp"
#include "msgui/policies/data/DefaultMemoryPolicy.hpp"
#include "msgui/fonts/Font5x7.hpp"

extern "C"
{
extern uint32_t _fs_flash_start;
}

msos::fs::RamFs ramfs;
UsartWriter writer;

// void image_drawer()
// {
//     msos::drivers::displays::SSD1306_I2C<board::interfaces::LCD_I2C> lcd;
//     msgui::Factory<decltype(lcd), msgui::policies::data::DefaultMemoryPolicy<uint8_t>, msgui::policies::chunk::ChunkPolicy, msgui::policies::chunk::SSD1308ChunkPolicyParameters> factory(lcd);
//     const auto font = factory.make_font<msgui::fonts::Font5x7>();
//     const auto text = factory.make_text("To kiedys", font, {0, 0});

//     constexpr static auto heart = factory.make_bitmap<15, 12>(
//         0, 0, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 0, 0,
//         0, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 0,
//         1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
//         1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
//         1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
//         0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0,
//         0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0,
//         0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0,
//         0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0,
//         0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0
//     );
//     msgui::Position heart_position = msgui::Position{72, 32};
//     auto heart_image = factory.make_image(heart_position, heart);

//     const auto window = factory.configure_window()
//                             .width(lcd.width())
//                             .height(lcd.height())
//                             .make(text, heart_image);
//     lcd.clear();

//     while (board::gpio::MID_KEY::read())
//     {
//         if (board::gpio::LEFT_KEY::read())
//         {
//             board::gpio::LED_RED::setLow();
//         }
//         else
//         {
//             board::gpio::LED_RED::setHigh();
//             if (heart_position.x > 0)
//             {
//                 heart_position.x -= 1;
//             }
//             else
//             {
//                 heart_position.x = 127;
//             }
//             heart_image.move(heart_position);
//         }

//         if (board::gpio::MID_KEY::read())
//         {
//             board::gpio::LED_YELLOW::setLow();
//         }
//         else
//         {
//             board::gpio::LED_YELLOW::setHigh();
//             if (heart_position.y > 0)
//             {
//                 heart_position.y -= 1;
//             }
//             else
//             {
//                 heart_position.y = 63;
//             }
//             heart_image.move(heart_position);
//         }

//         if (board::gpio::RIGHT_KEY::read())
//         {
//             board::gpio::LED_BLUE::setLow();
//         }
//         else
//         {
//             board::gpio::LED_BLUE::setHigh();
//             if (heart_position.x <= 123)
//             {
//                 heart_position.x += 1;
//             }
//             else
//             {
//                 heart_position.x = 0;
//             }
//             heart_image.move(heart_position);
//         }
//         window.draw();

//     }
//     lcd.clear();
// }

void kernel_process(void*)
{
    msos::fs::Vfs& vfs = msos::fs::Vfs::instance();
    vfs.mount_fs("/", &ramfs);
    ramfs.mkdir("rom", 1);
    ramfs.mkdir("bin", 1);
    ramfs.mkdir("dev", 1);

    uint8_t* romfs_disk = reinterpret_cast<uint8_t*>(&_fs_flash_start);
    msos::fs::RomFs romfs(romfs_disk);
    msos::drivers::DeviceFs devfs;

    msos::drivers::character::UsartDriver usart(0);
    usart.load();
    devfs.register_driver("tty1", usart);
    vfs.mount_fs("/rom", &romfs);
    vfs.mount_fs("/dev", &devfs);

    msos::apps::AppRegistry& apps = msos::apps::AppRegistry::get_instance();
    // apps.register_executable("test.bin", reinterpret_cast<std::size_t>(&image_drawer));
    vfs.mount_fs("/bin", &apps);

    spawn_exec("/bin/msos_shell.bin", NULL, NULL, 0, 2048);

    while (true)
    {
    }
}

int main()
{
    board::board_init();
    hal::core::Core::initializeClocks();

    // using Usart = board::interfaces::Usart1;
    // Usart::init(115200);
    // board::gpio::LED_GREEN::init(hal::gpio::Output::OutputPushPull, hal::gpio::Speed::Low);
    // board::gpio::LED_RED::init(hal::gpio::Output::OutputPushPull, hal::gpio::Speed::Low);
    // board::gpio::LED_YELLOW::init(hal::gpio::Output::OutputPushPull, hal::gpio::Speed::Low);
    // board::gpio::LED_BLUE::init(hal::gpio::Output::OutputPushPull, hal::gpio::Speed::Low);

    // board::gpio::LEFT_KEY::init(hal::gpio::Input::InputPullUpDown);
    // board::gpio::MID_KEY::init(hal::gpio::Input::InputPullUpDown);
    // board::gpio::RIGHT_KEY::init(hal::gpio::Input::InputPullUpDown);
    // board::gpio::LED_GREEN::setHigh();

    // Usart::on_data([](const uint8_t c)
    // {
    //     write_to_stdin(c);
    // });


    spawn_root_process(&kernel_process, NULL, 2048);

    while (true)
    {
    }

}

