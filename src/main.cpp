#include <board.hpp>
#include <hal/time/sleep.hpp>
#include <hal/core/core.hpp>
#include <hal/core/backupRegisters.hpp>
#include <eul/utils/string.hpp>
#include <string_view>

void call_me()
{
    using Usart = board::interfaces::Usart1;
    Usart::write("I was called :)\n");
}

extern "C"
{
    int get_address();
}

struct EndlineTag{};

constexpr EndlineTag endl;

template <typename UsartType>
class UsartWriter
{
public:
    using SelfType = UsartWriter<UsartType>;
  
    template <typename T>
    const SelfType& operator<<(const T& t) const
    {
        write(t); 
        return *this;
    }

    const SelfType& operator<<(const EndlineTag) const 
    {
        write("\n");
        return *this;
    }

    template <typename T>
    void writeln(const T& t) const
    {
        write(t);
        write("\n");
    }

    void write(const std::string_view& str) const 
    {
        SelfType::write(str);
    }
};

int main()
{
    board::board_init();   
    hal::core::Core::initializeClocks();
    using LED = board::gpio::LED_BLUE;
    LED::init(hal::gpio::Output::OutputPushPull, hal::gpio::Speed::Default);
    using Usart = board::interfaces::Usart1; 
    Usart::init(9600);
    
    UsartWriter<Usart> writer; 
    writer << "Hello from MSOS Kernel!" << endl;
    uint32_t address = reinterpret_cast<uint32_t>(&call_me);
    char data[30];
    eul::utils::itoa(address, data, 16);
    Usart::write("Address of function: 0x");
    Usart::write(data);
    Usart::write("\n");
    hal::core::BackupRegisters::init(); 
    hal::core::BackupRegisters::write(1, address >> 16);
    hal::core::BackupRegisters::write(2, address);

    uint32_t readed_address = get_address();
    eul::utils::itoa(readed_address, data, 16);
    Usart::write("Readed addres: 0x");
    Usart::write(data);
    Usart::write("\n");
    void(*call)()  = reinterpret_cast<void(*)()>(readed_address);
    hal::core::BackupRegisters::reset();
    readed_address = get_address();
    eul::utils::itoa(readed_address, data, 16);
    Usart::write("Readed addres: 0x");
    Usart::write(data);
    Usart::write("\n");
    
    std::size_t module_address = 0x08000000;
    module_address += 32 * 1024;

    std::string_view cookie(reinterpret_cast<const char*>(module_address), 4);
    Usart::write(cookie.data());
    Usart::write("\n");

    while (true)
    {
        call();
        LED::setHigh();
        hal::time::sleep(std::chrono::seconds(1));
        LED::setLow();
        hal::time::sleep(std::chrono::seconds(1));
        Usart::write("toggle\n");
    }
}

