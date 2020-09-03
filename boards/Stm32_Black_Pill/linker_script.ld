MEMORY
{
    FLASH (rx)   : ORIGIN = 0x08000000, LENGTH = 50K
    FS_FLASH(rx) : ORIGIN = 0x0800c800, LENGTH = 10K
    RAM (xrw)    : ORIGIN = 0x20000400, LENGTH = 19K
    KERNEL_RAM(rxw)  : ORIGIN = 0x20000000, LENGTH = 128B

}

_heap_size = 0x2500;
_stack_size = 0x2000;

_fs_flash_start = ORIGIN(FS_FLASH);
_fs_flash_end = ORIGIN(FS_FLASH) + LENGTH(FS_FLASH);
/*
_kernel_ram_start = ORIGIN(KERNEL_RAM);
_kernel_ram_end = ORIGIN(KERNEL_RAM) + LENGTH(KERNEL_RAM); */

INCLUDE devices/arm/stm32/stm32f1/stm32f10x_common.ld
