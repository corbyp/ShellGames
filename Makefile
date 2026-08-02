ENGINE_INCLUDE = engine/include
ENGINE_FILES = $(wildcard engine/src/*.c)

GAMES := evader collector snake jnr
GAME_FILES = $(wildcard $(GAME)/src/*.c)
TARGET = $(GAME)d

CFLAGS = -I$(ENGINE_INCLUDE) -Wall -Wextra
CFLAGS_RELEASE = $(CFLAGS) -O2 -DNDEBUG
CFLAGS_DEBUG = $(CFLAGS) -g -O0
CFLAGS_SANITIZE = $(CFLAGS) -fsanitize=address,undefined,leak -g

.PHONY: all run drun srun compile dcompile scompile exec gdb valgrind clean $(GAMES) coop

all: run

coop: coop/src/game.c
	$(CC) $(CFLAGS) coop/src/game.c $(ENGINE_FILES) -o bin/coop.bin

# production
compile:
	$(CC) $(CFLAGS_RELEASE) $(GAME_FILES) $(ENGINE_FILES) -o bin/$(TARGET)

exec:
	bin/$(TARGET)

# debug
dcompile:
	$(CC) $(CFLAGS_DEBUG) $(GAME_FILES) $(ENGINE_FILES) -o $(TARGET)

gdb:
	gdb $(TARGET)

# sanitize
scompile:
	$(CC) $(CFLAGS_SANITIZE) $(GAME_FILES) $(ENGINE_FILES) -o $(TARGET)

valgrind:
	valgrind ./$(TARGET)

# general
clean:
	rm -rf .cache bin/*

run: compile exec clean
drun: dcompile gdb clean
srun: scompile clean

$(GAMES):
	$(MAKE) run GAME=$@

build-%:
	$(MAKE) compile GAME=$*

san-%:
	$(MAKE) srun GAME=$*
