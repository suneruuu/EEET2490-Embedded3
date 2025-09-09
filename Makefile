#--------------------------------------Makefile-------------------------------------

CFILES   = $(wildcard ./src/*.c)
OFILES   = $(CFILES:./src/%.c=./build/%.o)
# + add include path and fno-builtin
GCCFLAGS = -Wall -O2 -ffreestanding -nostdinc -nostdlib -fno-builtin -I./includes

all: clean uart1_build kernel8.img run1
uart1: clean uart1_build kernel8.img run1
uart0: clean uart0_build kernel8.img run0

# Ensure build/ exists (Windows-friendly)
build:
	@if not exist build mkdir build

uart1_build: ./library/uart1.c | build
	aarch64-none-elf-gcc $(GCCFLAGS) -c ./library/uart1.c -o ./build/uart.o

uart0_build: ./library/uart0.c | build
	aarch64-none-elf-gcc $(GCCFLAGS) -c ./library/uart0.c -o ./build/uart.o

./build/boot.o: ./src/boot.S | build
	aarch64-none-elf-gcc $(GCCFLAGS) -c ./src/boot.S -o ./build/boot.o

./build/%.o: ./src/%.c | build
	aarch64-none-elf-gcc $(GCCFLAGS) -c $< -o $@

kernel8.img: ./build/boot.o ./build/uart.o $(OFILES)
	aarch64-none-elf-ld -nostdlib ./build/boot.o ./build/uart.o $(OFILES) -T ./src/link.ld -o ./build/kernel8.elf
	aarch64-none-elf-objcopy -O binary ./build/kernel8.elf ./build/kernel8.img

clean:
	-del .\build\kernel8.elf .\build\*.o .\build\*.img 2>NUL

# Run emulation with QEMU
run1:
	qemu-system-aarch64 -M raspi3 -kernel .\build\kernel8.img -serial null -serial stdio

run0:
	qemu-system-aarch64 -M raspi3 -kernel .\build\kernel8.img -serial stdio
