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

#include <cstdint>
#include <string_view>
#include <type_traits>
#include <unistd.h>

struct logging_flags
{
public:
    enum class base : uint8_t
    {
        dec,
        hex,
        oct,
        bin
    };

    enum class boolalpha : uint8_t
    {
        enabled,
        disabled
    };

    logging_flags();
    void set_base(const base new_base);
    base get_base() const;
    void set_boolalpha(const boolalpha new_boolalpha);
    boolalpha get_boolalpha() const;

private:
    base base_;
    boolalpha boolalpha_;
};

constexpr logging_flags::base dec = logging_flags::base::dec;
constexpr logging_flags::base hex = logging_flags::base::hex;
constexpr logging_flags::base oct = logging_flags::base::oct;
constexpr logging_flags::base bin = logging_flags::base::bin;

constexpr logging_flags::boolalpha boolalpha = logging_flags::boolalpha::enabled;

struct EndlineTag{};

constexpr EndlineTag endl;

class UsartWriter
{
public:
    template <typename T>
    UsartWriter& operator<<(const T& t)
    {
        write(t);
        return *this;
    }

    UsartWriter& operator<<(const EndlineTag);
    UsartWriter& operator<<(const logging_flags::base base);

    template <typename T>
    void writeln(const T& t)
    {
        write(t);
        write("%d\n");
    }

    void write(const std::string_view& str);

    template <typename T, typename std::enable_if_t<
        std::conjunction_v<std::is_integral<T>, std::is_signed<T>, std::negation<std::is_same<T, bool>>>, int> = 0>
    void write(T data)
    {
        int base = get_base();

        if (data < 0)
        {
            const char digit = {'-'};
            write(std::string_view(&digit, 1));

            data *= -1;
        }

        int zeros_at_end = reverse_number(data, base);
        write_number_as_text(data, base, zeros_at_end);
    }

    template <typename T, typename std::enable_if_t<
        std::conjunction_v<std::is_integral<T>, std::is_unsigned<T>, std::negation<std::is_same<T, bool>>>, int> = 0>
    void write(T data)
    {
        int base = get_base();

        int zeros_at_end = reverse_number(data, base);
        write_number_as_text(data, base, zeros_at_end);
    }

    void write(char data);

protected:
    template <typename T>
    void write_number_as_text(T number, int base, int zeros_at_end)
    {
        while (number != 0)
        {
            const char digit = {"0123456789abcdef"[number%base]};

            write(std::string_view(&digit, 1));

            number /= base;
        }
        while (zeros_at_end)
        {
            const char digit = {'0'};
            write(std::string_view(&digit, 1));

            --zeros_at_end;
        }
    }

    int get_base() const;

    template <typename T>
    int reverse_number(T& number, int base)
    {
        if (number == 0)
        {
            return 1;
        }

        T data = number;

        number = 0;
        int zeros_at_end = 0;
        bool is_zero_at_end = true;

        while (data != 0)
        {
            T temp = data % base;
            if (is_zero_at_end && temp == 0)
            {
                ++zeros_at_end;
            }
            else
            {
                is_zero_at_end = false;
            }
            number = number * base + temp;
            data /= base;
        }
        return zeros_at_end;
    }

    logging_flags flags_;
};

