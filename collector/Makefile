INCLUDE = include/
TEST_FILES = $(wildcard tests/*.c)
C_FILES = $(wildcard src/*.c)
TARGET = app

CFLAGS = -I$(INCLUDE) -Wall -Wextra
CFLAGS_RELEASE = $(CFLAGS) -O2 -DNDEBUG
CFLAGS_DEBUG = $(CFLAGS) -g -O0
CFLAGS_SANITIZE = $(CFLAGS) -fsanitize=address,undefined,leak -g

.PHONY: all run drun srun compile dcompile scompile exec gdb valgrind clean

all: run

# production
compile:
	$(CC) $(CFLAGS_RELEASE) $(TEST_FILES) $(C_FILES) -o $(TARGET)

exec:
	./$(TARGET)

# debug
dcompile:
	$(CC) $(CFLAGS_DEBUG) $(TEST_FILES) $(C_FILES) -o $(TARGET)

gdb:
	gdb $(TARGET)

#sanitize
scompile:
	$(CC) $(CFLAGS_SANITIZE) $(TEST_FILES) $(C_FILES) -o $(TARGET)

valgrind:
	valgrind ./$(TARGET)

# general
clean:
	rm -rf .cache $(TARGET)

run: compile exec clean
drun: dcompile gdb clean
srun: scompile exec compile valgrind clean