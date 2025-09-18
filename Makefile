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

# Toolchain
CC = aarch64-none-elf-gcc
LD = aarch64-none-elf-ld
OBJCOPY = aarch64-none-elf-objcopy

# Source files including game assets
SRC_CFILES = $(wildcard ./src/*.c) $(wildcard ./src/*/*.c)
# Exclude game_main.c from regular build since it has its own main function
# Exclude src versions of files that are in library to avoid duplicates
SRC_CFILES := $(filter-out ./src/game_main.c ./src/framebf.c ./src/mbox.c,$(SRC_CFILES))
GAME_ASSETS = $(wildcard ./assets/images/*.c)
LIB_CFILES = $(wildcard ./library/*.c) $(wildcard ./library/*/*.c)
# Avoid duplicate symbols and double-compiled UARTs and other duplicates
LIB_CFILES := $(filter-out ./library/peripheral/uart0.c ./library/peripheral/uart1.c ./library/uart0.c ./library/uart1.c,$(LIB_CFILES))
UTILS_CFILES = $(wildcard ./utils/*.c)

SRC_OFILES = $(SRC_CFILES:./src/%.c=./build/%.o)
LIB_OFILES = $(LIB_CFILES:./library/%.c=./build/library/%.o)
UTILS_OFILES = $(UTILS_CFILES:./utils/%.c=./build/utils/%.o)
OFILES = $(SRC_OFILES) $(LIB_OFILES) $(UTILS_OFILES)

GCCFLAGS = -Wall -O2 -ffreestanding -nostdinc -nostdlib -MMD -MP -I.

all:
	"$(MAKE)" clean
	"$(MAKE)" uart1_build
	"$(MAKE)" kernel8.img
	"$(MAKE)" run1
uart1: uart1_build kernel8.img run1
uart0: uart0_build kernel8.img run0

# Game targets
game: clean uart1_build game.img run_game

GAME_ASSET_OFILES = $(GAME_ASSETS:./assets/images/%.c=./build/assets/%.o)

game.img: ./build/boot.o ./build/uart.o ./build/game_main.o ./build/game.o ./build/framebf.o ./build/mbox.o ./build/game/map.o ./build/game/player.o ./build/game/sprites.o ./build/game/game_logic.o ./build/utils/string.o
	$(LD) -nostdlib $^ -T ./src/link.ld -o ./build/game.elf
	$(OBJCOPY) -O binary ./build/game.elf ./build/game.img

uart1_build: ./library/peripheral/uart1.c
	$(CC) $(GCCFLAGS) -c ./library/peripheral/uart1.c -o ./build/uart.o

uart0_build: ./library/peripheral/uart0.c
	$(CC) $(GCCFLAGS) -c ./library/peripheral/uart0.c -o ./build/uart.o

./build/boot.o: ./src/boot.S
	$(CC) $(GCCFLAGS) -c ./src/boot.S -o ./build/boot.o

./build/%.o: ./src/%.c
	@$(MKDIR)
	$(CC) $(GCCFLAGS) -c $< -o $@

./build/library/%.o: ./library/%.c
	@$(MKDIR)
	$(CC) $(GCCFLAGS) -c $< -o $@

./build/utils/%.o: ./utils/%.c
	@$(MKDIR)
	$(CC) $(GCCFLAGS) -c $< -o $@

# Game-specific object file compilation
./build/game.o: ./src/game/game.c
	@$(MKDIR)
	$(CC) $(GCCFLAGS) -c $< -o $@

./build/game/map.o: ./src/game/map.c
	@$(MKDIR)
	$(CC) $(GCCFLAGS) -c $< -o $@

./build/game/player.o: ./src/game/player.c
	@$(MKDIR)
	$(CC) $(GCCFLAGS) -c $< -o $@

./build/game/sprites.o: ./src/game/sprites.c
	@$(MKDIR)
	$(CC) $(GCCFLAGS) -c $< -o $@

./build/game/game_logic.o: ./src/game/game_logic.c
	@$(MKDIR)
	$(CC) $(GCCFLAGS) -c $< -o $@

# Game main compilation
./build/game_main.o: ./src/game_main.c
	@$(MKDIR)
	$(CC) $(GCCFLAGS) -c $< -o $@

# Game asset compilation
./build/assets/%.o: ./assets/images/%.c
	@$(MKDIR)
	$(CC) $(GCCFLAGS) -c $< -o $@

kernel8.img: ./build/boot.o ./build/uart.o $(OFILES)
	$(LD) -nostdlib ./build/boot.o ./build/uart.o $(OFILES) -T ./src/link.ld -o ./build/kernel8.elf
	$(OBJCOPY) -O binary ./build/kernel8.elf ./build/kernel8.img

clean:
	$(CLEAN_CMD)

# Run emulation with QEMU
run1: kernel8.img
	qemu-system-aarch64 -M raspi3 -kernel ./build/kernel8.img -serial null -serial stdio

run0: kernel8.img
	qemu-system-aarch64 -M raspi3 -kernel ./build/kernel8.img -serial stdio

run_game: game.img
	qemu-system-aarch64 -M raspi3 -kernel ./build/game.img -serial null -serial stdio

.PHONY: all uart1 uart0 uart1_build uart0_build clean run1 run0 game game.img run_game
