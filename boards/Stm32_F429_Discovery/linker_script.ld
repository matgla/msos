MEMORY
{
    FLASH (rx)   : ORIGIN = 0x08000000, LENGTH = 1024K
    FS_FLASH(rx) : ORIGIN = 0x08100000, LENGTH = 1024K
    RAM (xrw)    : ORIGIN = 0x20000000, LENGTH = 192K
    CCM(rxw)     : ORIGIN = 0x10000000, LENGTH = 64K

}

_heap_size = 0x2000;
_stack_size = 0x2000;

_fs_flash_start = ORIGIN(FS_FLASH);
_fs_flash_end = ORIGIN(FS_FLASH) + LENGTH(FS_FLASH);

_ram_start = ORIGIN(RAM);
_ram_end = ORIGIN(RAM) + LENGTH(RAM);

INCLUDE devices/arm/stm32/stm32_common.ld
