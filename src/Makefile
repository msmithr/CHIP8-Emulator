CC = gcc
CFLAGS = -std=gnu99 -Wall
LDFLAGS = -lSDL2

SOURCES = chip8.c cpu.c keyboard.c display.c
OBJECTS = $(SOURCES:.c=.o)

TARGET = emu

all: $(TARGET)
debug: CFLAGS += -DDEBUG
debug: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

.PHONY: clean

clean:
	@rm -f $(TARGET) $(OBJECTS) core
