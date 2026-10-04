#ifndef VMM_H
#define VMM_H

#include <stdint.h>

#define PAGE_SIZE 4096
#define PAGE_SHIFT 12

/* PTE Permission Flags */
#define PTE_V   (1UL << 0) // Valid
#define PTE_R   (1UL << 1) // Read
#define PTE_W   (1UL << 2) // Write
#define PTE_X   (1UL << 3) // Execute
#define PTE_U   (1UL << 4) // User mode accessible
#define PTE_G   (1UL << 5) // Global
#define PTE_A   (1UL << 6) // Accessed
#define PTE_D   (1UL << 7) // Dirty

/* Extract Virtual Page Numbers (VPN) */
#define VPN2(va) (((uintptr_t) (va) >> 30) & 0x1FF)
#define VPN1(va) (((uintptr_t) (va) >> 21) & 0x1FF)
#define VPN0(va) (((uintptr_t) (va) >> 12) & 0x1FF)

/* Convert Physical Address to Page Table PPN field & vice versa */
#define PA_TO_PTE(pa) ((((uintptr_t)(pa)) >> 12) << 10)
#define PTE_TO_PA(pte) ((((uintptr_t)(pte) & ~0x3FFUL) >> 10) << 12)

typedef uint64_t pte_t;
typedef pte_t pde_t;

#define SATP_MODE_SV39 (8UL << 60)
#define MAKE_SATP(page_table) (SATP_MODE_SV39 | (((uintptr_t)(page_table)) >> 12))

void vmm_init(void);
int map_page(pte_t *root, uintptr_t va, uintptr_t pa, uint64_t flags);

#endif