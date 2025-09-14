#--------------------------------------Makefile-------------------------------------

CFILES = $(filter-out ./src/game/game.c, $(wildcard ./src/*.c ./src/game/*.c ./assets/images/*.c))
OFILES = $(CFILES:./src/%.c=./build/%.o)
GCCFLAGS = -Wall -O2 -ffreestanding -nostdinc -nostdlib -I./includes

all: clean uart1_build kernel18.img run1
uart1: clean uart1_build kernel8.img run1
uart0: clean uart0_build kernel8.img run0

# Add a target for building game.img
game.img: ./build/boot.o ./build/uart.o ./build/game.o ./build/framebf.o ./build/mbox.o ./build/game/map.o ./build/game/player.o ./build/game/sprites.o 
	aarch64-none-elf-ld -nostdlib $^ -T ./src/link.ld -o ./build/game.elf
	aarch64-none-elf-objcopy -O binary ./build/game.elf ./build/game.img

./build/uart.o: ./library/uart1.c
	aarch64-none-elf-gcc $(GCCFLAGS) -c ./library/uart1.c -o ./build/uart.o

uart1_build: ./library/uart1.c
	aarch64-none-elf-gcc $(GCCFLAGS) -c ./library/uart1.c -o ./build/uart.o

uart0_build: ./library/uart0.c
	aarch64-none-elf-gcc $(GCCFLAGS) -c ./library/uart0.c -o ./build/uart.o

./build/boot.o: ./src/boot.S
	aarch64-none-elf-gcc $(GCCFLAGS) -c ./src/boot.S -o ./build/boot.o

./build/%.o: ./src/%.c
	@if not exist "$(dir $@)" mkdir "$(dir $@)"
	aarch64-none-elf-gcc $(GCCFLAGS) -c $< -o $@

./build/game.o: ./src/game/game.c
	aarch64-none-elf-gcc $(GCCFLAGS) -c $< -o $@

./build/game/map.o: ./src/game/map.c
	aarch64-none-elf-gcc $(GCCFLAGS) -c $< -o $@

./build/game/player.o: ./src/game/player.c
	aarch64-none-elf-gcc $(GCCFLAGS) -c $< -o $@

./build/game/sprites.o: ./src/game/sprites.c
	aarch64-none-elf-gcc $(GCCFLAGS) -c $< -o $@

kernel18.img: ./build/boot.o ./build/uart.o $(OFILES)
	aarch64-none-elf-ld -nostdlib ./build/boot.o ./build/uart.o $(OFILES) -T ./src/link.ld -o ./build/kernel18.elf
	aarch64-none-elf-objcopy -O binary ./build/kernel18.elf ./build/kernel18.img

clean:
	del /f /q .\build\*.elf .\build\*.o .\build\*.img

# Run emulation with QEMU
run1: 
	qemu-system-aarch64 -M raspi3 -kernel .\build\kernel18.img -serial null -serial stdio

run0: 
	qemu-system-aarch64 -M raspi3 -kernel .\build\kernel18.img -serial stdio

run_game: 
	qemu-system-aarch64 -M raspi3 -kernel .\build\game.img -serial null -serial stdio

game: clean game.img run_game
