#include <type_traits>

#include <cstring>

extern "C"
{
    void usart_write(const char* data);
}

static void reverse(char* s)
{
    char* j = s + std::strlen(s) - 1;
    int c;

    j = s + std::strlen(s) - 1;
    while (s < j)
    {
        c    = *s;
        *s++ = *j;
        *j-- = c;
    }
}

static char int_to_char(int n)
{
    if (n > 9)
    {
        return n - 10 + 'a';
    }
    else
    {
        return n + '0';
    }
}

template <typename T>
static inline T itoa(T n, char* s, int base_n = 10)
{
    static_assert(std::is_arithmetic<T>::value, "Type provided for serialize isn't arithmetic");
    T i, sign;

    if ((sign = n) < 0) /* record sign */
        n = -n;         /* make n positive */
    i = 0;
    do
    {                                   /* generate digits in reverse order */
        s[i++] = int_to_char(n % base_n); /* get next digit */
    } while ((n /= base_n) > 0);        /* delete it */
    if (sign < 0)
        s[i++] = '-';
    s[i] = '\0';
    reverse(s);
    return i;
}

int global_integer = 177176;

int main() 
{
    global_integer = 177177;
    static const char* local_string = "Module started\n";
    usart_write(local_string);

    int local_int = -123456;
    constexpr int const_int = 21234;
    char data_buffer[30];
    itoa(local_int, data_buffer);
    static const char* integer_text = "Integer: ";
    usart_write(integer_text);
    usart_write(data_buffer);
    usart_write("\n");
    static const char* constexpr_text = "Constexpr integer: ";
    usart_write(constexpr_text);
    itoa(const_int, data_buffer);
    usart_write(data_buffer);
    usart_write("\n");
    static const char* global_text = "Global integer: ";
    usart_write(global_text);
    itoa(global_integer, data_buffer);
    usart_write(data_buffer);
    usart_write("\n");

}

