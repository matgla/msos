void write_to_usart(const char* data);

int main()
{
    write_to_usart("Hello from module\nI am very happy :)\n");
    write_to_usart("But why not escaping? :(\n");
    return 0;
}

