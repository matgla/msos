extern "C"
{
    void usart_write(const char* data);
}

int main()
{
    usart_write("Hello from simple single module\n");
}


