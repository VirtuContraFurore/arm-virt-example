CROSS_COMPILE = aarch64-linux-gnu-
CC = $(CROSS_COMPILE)gcc
LD = $(CROSS_COMPILE)ld
AS = $(CROSS_COMPILE)as


SRC_FILES := $(wildcard src/*.c)
OBJ_FILES := $(SRC_FILES:src/%.c=build/%.o)
INCLUDES = -Iinclude

all: hypervisor.elf

build/start.o: src/start.s
	$(AS) -g -c $< -o $@

build/%.o: src/%.c
	$(CC) $(INCLUDES) -c $< -o $@

hypervisor.elf: build/start.o $(OBJ_FILES) linker.ld
	$(LD) -Tlinker.ld build/start.o $(OBJ_FILES) -o $@

clean:
	rm -f build/start.o $(OBJ_FILES) hypervisor.elf

.PHONY: all clean
