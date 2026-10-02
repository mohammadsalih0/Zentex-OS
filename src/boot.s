.attribute arch, "rv64gc"
.option norvc

.section .text.entry
.globl _start

_start:
    # Disable interrupts
    csrw sstatus, zero
    csrw sie, zero

    # Read current HARD ID (a0 contains hartid from OpenSBI)
    # Park secondary HARTs for now
    bnez a0, .Lpark

    # Setup stack pointer
    la sp, boot_stack_top

    # Jump to C entry point (main.c)
    call kmain

.Lpark:
    wfi
    j .Lpark

.section .bss
.align 12
.globl boot_stack_bottom

boot_stack_bottom:
    .skip 16384     # 16 KiB boot stack
.globl boot_stack_top
boot_stack_top: