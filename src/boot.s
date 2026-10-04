.attribute arch, "rv64gc"
.option norvc

.section .text.boot
.globl _start

_start:
    # Disable interrupts
    csrw sie, zero

    # Set up stack pointer
    la sp, stack_top

    # Zero .bss section
    la t0, _bss_start
    la t1, _bss_end
1:
    bgeu t0, t1, 2f
    sd zero, 0(t0)
    addi t0, t0, 8
    j 1b

2:
    # Pass OpenSBI parameters to kmaain (a0 = hartid, a1 = dtb)
    call kmain

3: wfi
    j 1b

.section .bss
.align 16
stack_bottom:
    .skip 16384
stack_top: