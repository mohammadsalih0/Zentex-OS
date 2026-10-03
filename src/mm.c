#include "mm.h"
#include "kprintf.h"

extern char _kernel_end[];

#define RAM_END 0x88000000L // 128MB RAM limit on QEMU virt (0x80000000 + 128M)

static uintptr_t next_free_page;

void mm_init(void) {
    uintptr_t raw_end = (uintptr_t)_kernel_end;
    next_free_page = (raw_end + PAGE_SIZE -1) & ~(PAGE_SIZE - 1);
    kprintf("[MM] Physical allocator initialized. Free RAM starting at: %p\n", (void *)next_free_page);
}

void *kalloc(void) {
    if (next_free_page + PAGE_SIZE > RAM_END) {
        kprintf("[MM] Out of memory!\n");
        return (void*)0;
    }

    void *ptr = (void *)next_free_page;
    next_free_page += PAGE_SIZE;

    // Zero out allocated page
    uint8_t *p = (uint8_t *)ptr;
    for (uint32_t i = 0; i < PAGE_SIZE; i++) {
        p[i] = 0;
    }

    return ptr;
}

void kfree(void *ptr) {
    (void)ptr; // Simple bump allocator; full page tracking can be added with a free list later
}