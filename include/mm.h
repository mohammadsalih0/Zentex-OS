#ifndef MM_H
#define MM_H

#include <stdint.h>

#define PAGE_SIZE 4096

void mm_init(void);
void *kalloc(void);
void kfree(void *ptr);

#endif