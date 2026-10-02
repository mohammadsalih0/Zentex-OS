TARGET	?= zentex.elf
CROSS_PREFIX ?= riscv64-unknown-elf-

CC	:= $(CROSS_PREFIX)gcc
LD	:= $(CROSS_PREFIX)ld
OBJCOPY := $(CROSS_PREFIX)objcopy

#Flags

CFLAGS	:= -Wall -Wextra -pedantic -std=c11 \
			-mcmodel=medany -ffreestanding -nostdlib -fno-builtin \
			-mabi=lp64d -march=rv64gc \
			-Iinclude -g -O2

LDFLAGS	:= -T kernel.ld -nostdlib

SRCS_S := $(wildcard src/*.s)
SRCS_C := $(wildcard src/*.c)

OBJS	:= $(SRCS_S:.s=.o) $(SRCS_C:.c=.o)

.PHONY: all clean run

all: $(TARGET)

%.o: %.s
	$(CC) $(CFLAGS) -c $< -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET): $(OBJS) kernel.ld
	$(LD) $(LDFLAGS) $(OBJS) -o $@


run: $(TARGET)
	qemu-system-riscv64 \
		-machine virt \
		-cpu rv64 \
		-smp 1 \
		-m 128M \
		-bios default \
		-kernel $(TARGET) \
		-nographic

clean:
	rm -f src/*.o $(TARGET)