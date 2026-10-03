.attribute arch, "rv64gc"
.option norvc

.section .text.boot
.globl _start

_start:
    # Disable interrupts
    csrw sie, zero

    # Set up stack pointer
    la sp, stack_top

    # Pass OpenSBI parameters to kmaain (a0 = hartid, a1 = dtb)
    call kmain

1: wfi
    j 1b

.section .bss
.align 16
stack_bottom:
    .skip 16384
stack_top: