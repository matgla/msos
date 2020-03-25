MEMORY
{
    FLASH (rx)   : ORIGIN = 0x08000000, LENGTH = 32K
    FS_FLASH(rx) : ORIGIN = 0x08008000, LENGTH = 32K
    RAM (xrw)    : ORIGIN = 0x20000400, LENGTH = 19K
    KERNEL_RAM(rxw)  : ORIGIN = 0x20000000, LENGTH = 128B

}

_heap_size = 0x2500;
_stack_size = 0x2000;

INCLUDE ../../lib/hal/devices/arm/stm32/stm32f1/stm32f10x_common.ld
