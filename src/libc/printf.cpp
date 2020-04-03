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

#include <stdarg.h>
#include <stddef.h>
#include <cstdio>
#include <string_view>
#include <string_view>
#include <type_traits>
#include <cstdint>

#include <unistd.h>

#include <gsl/span>

#include "msos/usart_printer.hpp"
namespace
{

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

    logging_flags()
        : base_(base::dec)
        , boolalpha_(boolalpha::disabled)
    {
    }

    void set_base(const base new_base)
    {
        base_ = new_base;
    }

    base get_base() const
    {
        return base_;
    }

    void set_boolalpha(const boolalpha new_boolalpha)
    {
        boolalpha_ = new_boolalpha;
    }

    boolalpha get_boolalpha() const
    {
        return boolalpha_;
    }

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

class UsartWriterA
{
public:
    template <typename T>
    UsartWriterA& operator<<(const T& t)
    {
        write(t);
        return *this;
    }

    UsartWriterA& operator<<(const EndlineTag)
    {
        write("\n");
        return *this;
    }

    UsartWriterA& operator<<(const logging_flags::base base)
    {
        flags_.set_base(base);
        return *this;
    }

    template <typename T>
    void writeln(const T& t)
    {
        write(t);
        write("\n");
    }

    void write(const std::string_view& str)
    {
        ::write(1, str.data(), str.length());
    }

    template <typename T, typename std::enable_if_t<
        std::conjunction_v<std::is_integral<T>, std::is_signed<T>, std::negation<std::is_same<T, bool>>>, int> = 0>
    void write(T data)
    {
        int base = get_base();

        if (data < 0)
        {
            const char digit = '-';
            ::write(1, &digit, 1);

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

    void write(char data)
    {
        char buf[] = { data, '\0' };
        write(buf);
    }

protected:
    template <typename T>
    void write_number_as_text(T number, int base, int zeros_at_end)
    {
        while (number != 0)
        {
            const char digit = {"0123456789abcdef"[number%base]};
            ::write(1, &digit, 1);

            number /= base;
        }
        while (zeros_at_end)
        {
            const char digit = {'0'};
            ::write(1, &digit, 1);

            --zeros_at_end;
        }
    }

    int get_base() const
    {
        switch (flags_.get_base())
        {
            case logging_flags::base::dec:
            {
                return 10;
            } break;
            case logging_flags::base::hex:
            {
                return 16;
            } break;
            case logging_flags::base::oct:
            {
                return 8;
            } break;
            case logging_flags::base::bin:
            {
                return 2;
            } break;
        }
        return 0;
    }

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
            int temp = data % base;
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

}

static UsartWriterA writer_a_;
static UsartWriter writer_b_;


int __vfscanf_( int fd, const char* format, va_list argptr,
                    unsigned int mode_flags)
{
    if (std::string_view(format) == "%s")
    {
        char* buffer = va_arg(argptr, char*);
        return read(fd, buffer, 0);
    }
    return 0;
}

int _scanf(const char* format, ...)
{
    va_list arg;
    va_start (arg, format);
    int done = __vfscanf_(0, format, arg, 0);
    va_end (arg);
    return done;
}

template <typename T>
int __vfprintf_(T& writer, int fd, const char* format, va_list argptr,
                    unsigned int mode_flags)
{
    // if (std::string_view(format) == "%s")
    // {
    //     char* buffer = va_arg(argptr, char*);
    //     return read(fd, buffer, 0);
    // }
    std::string_view data(format);
    int position = 0, previous_position = 0, size = 0;
    //  %[flags][width][.precision][length]specifier
    do
    {
        int position = data.find("%");
        if (position == std::string_view::npos)
        {
            writer << std::string_view(data.data() + previous_position, data.length());
            return size + data.length();
        }
        if (position > 0)
        {
            writer << data.substr(previous_position, position);
            size += data.substr(previous_position, position).length();
        }
        if (position + 1 >= data.length())
        {
            return size + data.length();
        }
        ++position;
        char specifier = data[position];
        switch (specifier)
        {
            case 's': {
                const char* data = va_arg(argptr, const char*);
                writer << data;
            } break;
            case 'd': {
                int data = va_arg(argptr, int);
                writer << data;
            }
        }
        if (position + 1 >= data.length())
        {
            return size + data.length();
        }
        ++position;

        data = data.substr(position, data.length());
    } while (position != std::string_view::npos);
    return size;
}


int _printf(const char* format, ...)
{
    va_list arg;
    va_start (arg, format);
    int done = __vfprintf_(writer_a_, 1, format, arg, 0);
    va_end (arg);
    return done;
}

int _printf_via_usart(const char* format, ...)
{
    va_list arg;
    va_start (arg, format);
    int done = __vfprintf_(writer_b_, 1, format, arg, 0);
    va_end (arg);
    return 1;
}


// 2}
