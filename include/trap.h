#ifndef TRAP_H
#define TRAP_H

#include <stdint.h>

struct trap_frame
{
    uint64_t ra;
    uint64_t t0, t1, t2;
    uint64_t a0, a1, a2, a3;
    uint64_t a4, a5, a6, a7;
    uint64_t pad[16];

    uint64_t sepc;
    uint64_t sstatus;
    uint64_t stval;
    uint64_t scause;
};

void trap_init(void);

#endif
