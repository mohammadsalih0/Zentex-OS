.attribute arch, "rv64gc"
.option norvc

.section .text
.globl trap_vector
.type trap_vector, @function
.align 4        # stvec requires 4-byte alignment

trap_vector:
    # Save trap frame on current stack
    addi sp, sp, -256
    sd ra, 0(sp)
    sd t0, 32(sp)
    sd t1, 40(sp)
    sd t2, 48(sp)
    sd a0, 64(sp)
    sd a1, 72(sp)
    sd a2, 80(sp)
    sd a3, 88(sp)
    sd a4, 96(sp)
    sd a5, 104(sp)
    sd a6, 112(sp)
    sd a7, 120(sp)

    csrr t0, sepc
    csrr t1, sstatus
    csrr t2, stval
    csrr t3, scause

    sd t0, 224(sp)
    sd t1, 232(sp)
    sd t2, 240(sp)
    sd t3, 248(sp)

    mv a0, sp
    call trap_handler

    # Restore Registers
    ld t0, 224(sp)
    csrw sepc, t0

    ld ra, 0(sp)
    ld t0, 32(sp)
    ld t1, 40(sp)
    ld t2, 48(sp)
    ld a0, 64(sp)
    ld a1, 72(sp)
    ld a2, 80(sp)
    ld a3, 88(sp)
    ld a4, 96(sp)
    ld a5, 104(sp)
    ld a6, 112(sp)
    ld a7, 120(sp)

    addi sp, sp, 256
    sret
