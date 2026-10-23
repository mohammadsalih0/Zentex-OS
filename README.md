# Project Zentex OS

A modern, opinionated, Unix-like operating system kernel targeted at the RISC-V 64-bit architecture (`rv64gc`). Built to explore lightweight systems design, clean abstraction boundaries, and async-first subsystem workflows without legacy x86 baggage.

> [!NOTE]  
> **Development Status & Specification Notice:**  
> This project is an ongoing research and hobby kernel implementation. The architectural features, system call interfaces, and technical specifications outlined below represent current design goals. **They are actively evolving and are subject to change, redesign, or removal as implementation progresses.**

---

## Planned Technical Architecture

### 1. Execution Environment & Boot Pipeline
* **Target Architecture:** RISC-V 64-bit (`rv64gc`) on QEMU `virt` machine topology.
* **Privilege Level Management:** Executes in **Supervisor Mode (S-Mode)**, leveraging **OpenSBI** in Machine Mode (M-Mode) for early hardware isolation and firmware calls.
* **Clean Boot Interface:** Direct handoff from SBI to kernel `_start` in standard 64-bit mode with DTB (Device Tree Blob) parsing.

### 2. Virtual Memory & Address Space (Sv39 MMU)
* **Paging Scheme:** 3-level page table hierarchy mapping 39-bit virtual addresses to a 56-bit physical address space.
* **Strict Memory Protection:** Clear privilege boundaries enforced via Page Table Entries (PTE) with explicit Read/Write/Execute (`R/W/X`) and User (`U`) permission flags.
* **Higher-Half Kernel Layout:** Clean separation of kernel memory space and U-mode task user space.

### 3. Physical Memory Management
* **Page Allocator:** Frame-based physical memory management utilizing a linked-list free-list allocator (`kalloc` / `kfree`).
* **Slab/Buddy Allocator (Planned):** Variable-sized heap allocation for dynamic kernel data structures.

### 4. Trap, Interrupt & Syscall Infrastructure
* **Vector Architecture:** Uniform trap handler (`trap_vector`) managing context switches, CSR register saves/restores, and Supervisor-level exceptions.
* **Clean ABI Syscall Layer:** Non-blocking syscall dispatch mechanism handling U-mode `ecall` requests (`sys_write`, `sys_exit`, `sys_yield`, `sys_getpid`).
* **Interrupt Controllers:** Native integration with CLINT (timer interrupts) and PLIC (external device interrupts).

---

## Target System Roadmap & Specs

| Feature Subsystem | Current State / Target Spec | Status |
| :--- | :--- | :--- |
| **ISA Architecture** | RV64GC (`rv64imafdc`) | 🟡 Active Design |
| **Firmware Handoff** | OpenSBI (S-Mode Kernel Entry at `0x80200000`) | 🟡 Active Design |
| **MMU Model** | Sv39 Paging with 4KiB Granularity | 🟡 Active Design |
| **Process Model** | Isolated User Task Contexts via `sret` | 📋 Planned |
| **IPC / Async I/O** | Lockless Ring Buffer Channels (`io_uring` style) | 💡 Experimental |
| **Storage / VFS** | RAM-backed VFS / VirtIO Block Driver | 💡 Experimental |

---

## Building and Running (QEMU Target)

### Prerequisites
* `riscv64-unknown-elf-gcc` or `clang` cross-compiler toolchain
* `qemu-system-riscv64`
* `make` / `cmake`

### Quickstart
```bash
# Clone the repository
git clone [https://github.com/mohammadsalih0/Zentex-OS.git](https://github.com/mohammadsalih0/Zentex-OS.git)
cd your-os-repo

# Build the kernel binary
make

# Boot inside QEMU
make run
