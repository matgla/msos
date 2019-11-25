MEMORY
{
    FLASH (rx)   : ORIGIN = 0x08000000, LENGTH = 64K
    RAM (xrw)    : ORIGIN = 0x20000000, LENGTH = 20K
}

_heap_size = 0x2400;
_stack_size = 0x2400;

INCLUDE ../../lib/hal/devices/arm/stm32/stm32f1/stm32f10x_common.ld
