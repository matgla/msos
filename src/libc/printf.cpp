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

#include "msos/usart_printer.hpp"

// extern "C"
// {

// void _putchar(char character)
// {
//     UsartWriter writer;
//     writer << character;
// }

// 	int _vprintf_r(struct _reent reent, const char fmt,
//                         va_list list)
// {

// }
// 	int _vfprintf_r(struct _reent reent, FILE fp,
//                         const char fmt, va_list list)
//                         {

//                         }
// 	int _vsprintf_r(struct _reent reent, char str,
//                         const char fmt, va_list list)
//                         {

//                         }
// 	int _vasprintf_r(struct _reent reent, char *str,
//                          const char fmt, va_list list)
//                          {

//                          }
// 	int _vsnprintf_r(struct _reent reent, char str,
//                          size_t size, const char fmt, va_list list)
//                          {

//                          }
// 	char *_vasnprintf_r(struct _reent reent, char str,
//                             size_t size, const char fmt, va_list list)
//                             {

//                             }

// // static int _vsnprintf(out_fct_type out, char* buffer, const size_t maxlen, const char* format, va_list va)
// // {
// //   unsigned int flags, width, precision, n;
// //   size_t idx = 0U;

// //   if (!buffer) {
// //     // use null output function
// //     out = _out_null;
// //   }

// //   while (*format)
// //   {
// //     // format specifier?  %[flags][width][.precision][length]
// //     if (*format != '%') {
// //       // no
// //       out(*format, buffer, idx++, maxlen);
// //       format++;
// //       continue;
// //     }
// //     else {
// //       // yes, evaluate it
// //       format++;
// //     }

// //     // evaluate flags
// //     flags = 0U;
// //     do {
// //       switch (*format) {
// //         case '0': flags |= FLAGS_ZEROPAD; format++; n = 1U; break;
// //         case '-': flags |= FLAGS_LEFT;    format++; n = 1U; break;
// //         case '+': flags |= FLAGS_PLUS;    format++; n = 1U; break;
// //         case ' ': flags |= FLAGS_SPACE;   format++; n = 1U; break;
// //         case '#': flags |= FLAGS_HASH;    format++; n = 1U; break;
// //         default :                                   n = 0U; break;
// //       }
// //     } while (n);

// //     // evaluate width field
// //     width = 0U;
// //     if (_is_digit(*format)) {
// //       width = _atoi(&format);
// //     }
// //     else if (*format == '*') {
// //       const int w = va_arg(va, int);
// //       if (w < 0) {
// //         flags |= FLAGS_LEFT;    // reverse padding
// //         width = (unsigned int)-w;
// //       }
// //       else {
// //         width = (unsigned int)w;
// //       }
// //       format++;
// //     }

// //     // evaluate precision field
// //     precision = 0U;
// //     if (*format == '.') {
// //       flags |= FLAGS_PRECISION;
// //       format++;
// //       if (_is_digit(*format)) {
// //         precision = _atoi(&format);
// //       }
// //       else if (*format == '*') {
// //         const int prec = (int)va_arg(va, int);
// //         precision = prec > 0 ? (unsigned int)prec : 0U;
// //         format++;
// //       }
// //     }

// //     // evaluate length field
// //     switch (*format) {
// //       case 'l' :
// //         flags |= FLAGS_LONG;
// //         format++;
// //         if (*format == 'l') {
// //           flags |= FLAGS_LONG_LONG;
// //           format++;
// //         }
// //         break;
// //       case 'h' :
// //         flags |= FLAGS_SHORT;
// //         format++;
// //         if (*format == 'h') {
// //           flags |= FLAGS_CHAR;
// //           format++;
// //         }
// //         break;
// // #if defined(PRINTF_SUPPORT_PTRDIFF_T)
// //       case 't' :
// //         flags |= (sizeof(ptrdiff_t) == sizeof(long) ? FLAGS_LONG : FLAGS_LONG_LONG);
// //         format++;
// //         break;
// // #endif
// //       case 'j' :
// //         flags |= (sizeof(intmax_t) == sizeof(long) ? FLAGS_LONG : FLAGS_LONG_LONG);
// //         format++;
// //         break;
// //       case 'z' :
// //         flags |= (sizeof(size_t) == sizeof(long) ? FLAGS_LONG : FLAGS_LONG_LONG);
// //         format++;
// //         break;
// //       default :
// //         break;
// //     }

// //     // evaluate specifier
// //     switch (*format) {
// //       case 'd' :
// //       case 'i' :
// //       case 'u' :
// //       case 'x' :
// //       case 'X' :
// //       case 'o' :
// //       case 'b' : {
// //         // set the base
// //         unsigned int base;
// //         if (*format == 'x' || *format == 'X') {
// //           base = 16U;
// //         }
// //         else if (*format == 'o') {
// //           base =  8U;
// //         }
// //         else if (*format == 'b') {
// //           base =  2U;
// //         }
// //         else {
// //           base = 10U;
// //           flags &= ~FLAGS_HASH;   // no hash for dec format
// //         }
// //         // uppercase
// //         if (*format == 'X') {
// //           flags |= FLAGS_UPPERCASE;
// //         }

// //         // no plus or space flag for u, x, X, o, b
// //         if ((*format != 'i') && (*format != 'd')) {
// //           flags &= ~(FLAGS_PLUS | FLAGS_SPACE);
// //         }

// //         // ignore '0' flag when precision is given
// //         if (flags & FLAGS_PRECISION) {
// //           flags &= ~FLAGS_ZEROPAD;
// //         }

// //         // convert the integer
// //         if ((*format == 'i') || (*format == 'd')) {
// //           // signed
// //           if (flags & FLAGS_LONG_LONG) {
// // #if defined(PRINTF_SUPPORT_LONG_LONG)
// //             const long long value = va_arg(va, long long);
// //             idx = _ntoa_long_long(out, buffer, idx, maxlen, (unsigned long long)(value > 0 ? value : 0 - value), value < 0, base, precision, width, flags);
// // #endif
// //           }
// //           else if (flags & FLAGS_LONG) {
// //             const long value = va_arg(va, long);
// //             idx = _ntoa_long(out, buffer, idx, maxlen, (unsigned long)(value > 0 ? value : 0 - value), value < 0, base, precision, width, flags);
// //           }
// //           else {
// //             const int value = (flags & FLAGS_CHAR) ? (char)va_arg(va, int) : (flags & FLAGS_SHORT) ? (short int)va_arg(va, int) : va_arg(va, int);
// //             idx = _ntoa_long(out, buffer, idx, maxlen, (unsigned int)(value > 0 ? value : 0 - value), value < 0, base, precision, width, flags);
// //           }
// //         }
// //         else {
// //           // unsigned
// //           if (flags & FLAGS_LONG_LONG) {
// // #if defined(PRINTF_SUPPORT_LONG_LONG)
// //             idx = _ntoa_long_long(out, buffer, idx, maxlen, va_arg(va, unsigned long long), false, base, precision, width, flags);
// // #endif
// //           }
// //           else if (flags & FLAGS_LONG) {
// //             idx = _ntoa_long(out, buffer, idx, maxlen, va_arg(va, unsigned long), false, base, precision, width, flags);
// //           }
// //           else {
// //             const unsigned int value = (flags & FLAGS_CHAR) ? (unsigned char)va_arg(va, unsigned int) : (flags & FLAGS_SHORT) ? (unsigned short int)va_arg(va, unsigned int) : va_arg(va, unsigned int);
// //             idx = _ntoa_long(out, buffer, idx, maxlen, value, false, base, precision, width, flags);
// //           }
// //         }
// //         format++;
// //         break;
// //       }
// // #if defined(PRINTF_SUPPORT_FLOAT)
// //       case 'f' :
// //       case 'F' :
// //         if (*format == 'F') flags |= FLAGS_UPPERCASE;
// //         idx = _ftoa(out, buffer, idx, maxlen, va_arg(va, double), precision, width, flags);
// //         format++;
// //         break;
// // #if defined(PRINTF_SUPPORT_EXPONENTIAL)
// //       case 'e':
// //       case 'E':
// //       case 'g':
// //       case 'G':
// //         if ((*format == 'g')||(*format == 'G')) flags |= FLAGS_ADAPT_EXP;
// //         if ((*format == 'E')||(*format == 'G')) flags |= FLAGS_UPPERCASE;
// //         idx = _etoa(out, buffer, idx, maxlen, va_arg(va, double), precision, width, flags);
// //         format++;
// //         break;
// // #endif  // PRINTF_SUPPORT_EXPONENTIAL
// // #endif  // PRINTF_SUPPORT_FLOAT
// //       case 'c' : {
// //         unsigned int l = 1U;
// //         // pre padding
// //         if (!(flags & FLAGS_LEFT)) {
// //           while (l++ < width) {
// //             out(' ', buffer, idx++, maxlen);
// //           }
// //         }
// //         // char output
// //         out((char)va_arg(va, int), buffer, idx++, maxlen);
// //         // post padding
// //         if (flags & FLAGS_LEFT) {
// //           while (l++ < width) {
// //             out(' ', buffer, idx++, maxlen);
// //           }
// //         }
// //         format++;
// //         break;
// //       }

// //       case 's' : {
// //         const char* p = va_arg(va, char*);
// //         unsigned int l = _strnlen_s(p, precision ? precision : (size_t)-1);
// //         // pre padding
// //         if (flags & FLAGS_PRECISION) {
// //           l = (l < precision ? l : precision);
// //         }
// //         if (!(flags & FLAGS_LEFT)) {
// //           while (l++ < width) {
// //             out(' ', buffer, idx++, maxlen);
// //           }
// //         }
// //         // string output
// //         while ((*p != 0) && (!(flags & FLAGS_PRECISION) || precision--)) {
// //           out(*(p++), buffer, idx++, maxlen);
// //         }
// //         // post padding
// //         if (flags & FLAGS_LEFT) {
// //           while (l++ < width) {
// //             out(' ', buffer, idx++, maxlen);
// //           }
// //         }
// //         format++;
// //         break;
// //       }

// //       case 'p' : {
// //         width = sizeof(void*) * 2U;
// //         flags |= FLAGS_ZEROPAD | FLAGS_UPPERCASE;
// // #if defined(PRINTF_SUPPORT_LONG_LONG)
// //         const bool is_ll = sizeof(uintptr_t) == sizeof(long long);
// //         if (is_ll) {
// //           idx = _ntoa_long_long(out, buffer, idx, maxlen, (uintptr_t)va_arg(va, void*), false, 16U, precision, width, flags);
// //         }
// //         else {
// // #endif
// //           idx = _ntoa_long(out, buffer, idx, maxlen, (unsigned long)((uintptr_t)va_arg(va, void*)), false, 16U, precision, width, flags);
// // #if defined(PRINTF_SUPPORT_LONG_LONG)
// //         }
// // #endif
// //         format++;
// //         break;
// //       }

// //       case '%' :
// //         out('%', buffer, idx++, maxlen);
// //         format++;
// //         break;

// //       default :
// //         out(*format, buffer, idx++, maxlen);
// //         format++;
// //         break;
// //     }
// //   }

// //   // termination
// //   out((char)0, buffer, idx < maxlen ? idx : maxlen - 1U, maxlen);

// //   // return written chars without terminating \0
// //   return (int)idx;
// // }

int _printf(const char* format, ...)
{
    UsartWriter writer;
    writer << format;
    return 0;
}


// 2}
