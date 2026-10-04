OUTPUT_ARCH("riscv")
ENTRY(_start)

SECTIONS
{
    /* OpenSBI transfers execution to supervisor mode at 0x80200000 */
    . = 0x80200000;
    
    .text : AT(0x80200000) {
        *(.text.boot)
        *(.text .text.*)
        . = ALIGN(0x1000);
        _etext = .;
    }

    .rodata : {
        . = ALIGN(0X1000);
        _srodata = .;
        *(.rodata .rodata.*)
        . = ALIGN(0x1000);
        _erodata = .;
    }

    .data : {
        . = ALIGN(0x1000);
        _sdata = .;
        *(.data .data.*)
        . = ALIGN(0x1000);
        _edata = .;
    }

    .bss : {
        . = ALIGN(0x01000);
        _sbss = .;
        _bss_start = .;
        *(.bss .bss.*)
        *(COMMON)
        . = ALIGN(0x1000);
        _ebss = .;
        _bss_end = .;
    }
    . = ALIGN(0x1000);
    _kernel_end = .;
    _end = .;
}