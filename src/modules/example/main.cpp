
 void write_to_usart(const char* data);


int object_main()
{
    write_to_usart("Hello from module");
    return 0;
}
