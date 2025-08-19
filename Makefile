# Detect OS and shell environment
ifeq ($(OS),Windows_NT)
    # Check if we're in Git Bash by checking if rm command works
    ifeq ($(shell which rm 2>/dev/null),)
        # Windows Command Prompt (no rm command)
        RM = del /Q
        MKDIR = if not exist "$(@D)" mkdir "$(@D)" 2>nul
        CLEAN_CMD = del /Q .\build\kernel8.elf .\build\*.o .\build\*.img 2>nul || echo Clean completed
    else
        # Git Bash on Windows (rm command available)
        RM = rm -f
        MKDIR = mkdir -p $(@D)
        CLEAN_CMD = rm -f ./build/kernel8.elf ./build/*.o ./build/*.img 2>/dev/null || true
    endif
else
    # Unix/Linux/Git Bash
    RM = rm -f
    MKDIR = mkdir -p $(@D)
    CLEAN_CMD = rm -f ./build/kernel8.elf ./build/*.o ./build/*.img 2>/dev/null || true
endif

SRC_CFILES = $(wildcard ./src/*.c) $(wildcard ./src/*/*.c)
LIB_CFILES = $(wildcard ./library/*.c) $(wildcard ./library/*/*.c)
# Avoid duplicate symbols and double-compiled UARTs
LIB_CFILES := $(filter-out ./library/peripheral/uart0.c ./library/peripheral/uart1.c ./library/framebf.c,$(LIB_CFILES))
UTILS_CFILES = $(wildcard ./utils/*.c)

SRC_OFILES = $(SRC_CFILES:./src/%.c=./build/%.o)
LIB_OFILES = $(LIB_CFILES:./library/%.c=./build/library/%.o)
UTILS_OFILES = $(UTILS_CFILES:./utils/%.c=./build/utils/%.o)
OFILES = $(SRC_OFILES) $(LIB_OFILES) $(UTILS_OFILES)

GCCFLAGS = -Wall -O2 -ffreestanding -nostdinc -nostdlib -MMD -MP -I.

all: clean uart1_build kernel8.img run1
uart1: clean uart1_build kernel8.img run1
uart0: clean uart0_build kernel8.img run0

#./build/uart.o: ./library/uart1.c
#	aarch64-none-elf-gcc $(GCCFLAGS) -c ./uart/uart1.c -o ./build/uart.o

uart1_build: ./library/peripheral/uart1.c
	aarch64-none-elf-gcc $(GCCFLAGS) -c ./library/peripheral/uart1.c -o ./build/uart.o

uart0_build: ./library/peripheral/uart0.c
	aarch64-none-elf-gcc $(GCCFLAGS) -c ./library/uart0.c -o ./build/uart.o

./build/boot.o: ./src/boot.S
	aarch64-none-elf-gcc $(GCCFLAGS) -c ./src/boot.S -o ./build/boot.o

./build/%.o: ./src/%.c
	@$(MKDIR)
	aarch64-none-elf-gcc $(GCCFLAGS) -c $< -o $@

./build/library/%.o: ./library/%.c
	@$(MKDIR)
	aarch64-none-elf-gcc $(GCCFLAGS) -c $< -o $@

./build/utils/%.o: ./utils/%.c
	@$(MKDIR)
	aarch64-none-elf-gcc $(GCCFLAGS) -c $< -o $@

kernel8.img: ./build/boot.o ./build/uart.o $(OFILES)
	aarch64-none-elf-ld -nostdlib ./build/boot.o ./build/uart.o $(OFILES) -T ./src/link.ld -o ./build/kernel8.elf
	aarch64-none-elf-objcopy -O binary ./build/kernel8.elf ./build/kernel8.img

clean:
	$(CLEAN_CMD)

# Run emulation with QEMU
run1: 
	qemu-system-aarch64 -M raspi3 -kernel ./build/kernel8.img -serial null -serial stdio

run0: 
	qemu-system-aarch64 -M raspi3 -kernel ./build/kernel8.img -serial stdio