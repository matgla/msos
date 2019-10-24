MEMORY
{
    FLASH (rx)      : ORIGIN = 0x08000000, LENGTH = 128K
    RAM (xrw)       : ORIGIN = 0x20000000, LENGTH = 20K
}

_heap_size = 0x0;   /* required heap size */
_stack_size = 0x0;  /* required stack size */

INCLUDE ../../lib/hal/devices/arm/stm32/stm32f1/stm32f10x_common.ld