#include "vmm.h"
#include "mm.h"
#include "kprintf.h"
#include "string.h"
// Kernel root page table
static pte_t *kernel_root_page_table;

int map_page(pte_t *root, uintptr_t va, uintptr_t pa, uint64_t flags) {
    pte_t *current_table = root;
    uint64_t vpns[3] = {VPN0(va), VPN1(va), VPN2(va)};

    // Traverse Level 2 and Level 1 page tables
    for (int level = 2;  level > 0; level--) {
        uint64_t vpn = vpns[level];
        pte_t pte = current_table[vpn];

        if (!(pte & PTE_V)) {
            // Allocate a new page for the next level table
            void *next_table = kalloc();
            if (!next_table) {
                return -1;
            }
            memset(next_table, 0, PAGE_SIZE);

            // Create valid PTE and store it
            pte = PA_TO_PTE((uintptr_t)next_table) | PTE_V;
            current_table[vpn] = pte;
        }
        
        // Extract physical address of next level table using updated pte
        current_table = (pte_t *)PTE_TO_PA(pte);
    }

    // Set leaf PTE at Level 0
    uint64_t vpn0 = vpns[0];
    current_table[vpn0] = PA_TO_PTE(pa) | flags | PTE_V | PTE_A | PTE_D;

    return 0;
}

extern char _kernel_end[];

void vmm_init(void) {
    // 1. Allocate root page table page
    kernel_root_page_table = (pte_t *)kalloc();
    if (!kernel_root_page_table) {
        kprintf("[VMM ERROR] Failed to allocate root page table\n");
        return;
    }
    memset(kernel_root_page_table, 0, PAGE_SIZE);
    // 2. Identity Map MMIO UART (0x10000000)
    uintptr_t uart_addr = 0x10000000;
    map_page(kernel_root_page_table, uart_addr, uart_addr, PTE_R | PTE_W);

    for (uintptr_t addr = 0x80000000UL; addr < 0x80200000UL; addr += PAGE_SIZE) {
        map_page(kernel_root_page_table, addr, addr, PTE_R | PTE_W | PTE_X);
    }

    // 3. Identity Map Kernel Text, Data, BSS & Stack RAM (0x80200000 -> _kernel_end)
    uintptr_t start = 0x80200000UL;
    uintptr_t end = 0x81000000UL;

    for (uintptr_t addr = start; addr < end; addr += PAGE_SIZE) {
        if(map_page(kernel_root_page_table, addr, addr, PTE_R | PTE_W | PTE_X) < 0) {
            kprintf("[VMM ERROR] Failed to map virtual page %p\n", (void *)addr);
            return;
        }
    }

    kprintf("[VMM] Page tables created. Enabling SV39 MMU...\n");

    // 4. Write SATP register & flush TLB
    uintptr_t satp_val = MAKE_SATP(kernel_root_page_table);
    __asm__ __volatile__(
        "sfence.vma zero, zero\n"
        "csrw satp, %0\n"
        "sfence.vma zero, zero\n"
        :
        : "r"(satp_val)
        : "memory"
    );

    kprintf("[VMM] SV39 Virtual Memory initialized successfully.\n");
}