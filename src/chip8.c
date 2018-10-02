#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>
#include <SDL2/SDL.h>
#include "chip8.h"

#include "keyboard.h"
#include "display.h"
#include "cpu.h"

unsigned char fontset[80] = { 
    0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
    0x20, 0x60, 0x20, 0x20, 0x70, // 1
    0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
    0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
    0x90, 0x90, 0xF0, 0x10, 0x10, // 4
    0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
    0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
    0xF0, 0x10, 0x20, 0x40, 0x40, // 7
    0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
    0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
    0xF0, 0x90, 0xF0, 0x90, 0x90, // A
    0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
    0xF0, 0x80, 0x80, 0x80, 0xF0, // C
    0xE0, 0x90, 0x90, 0x90, 0xE0, // D
    0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
    0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};

// initialize a chip8 object, given the filepath to a rom file
static chip8 *initialize(char *rom) {
    int fd;
    chip8 *machine;

    // open the rom file
    if ((fd = open(rom, O_RDONLY)) == -1) {
        printf("%s\n", strerror(errno));
        return NULL;
    }

    // allocate memory for the chip8 object 
    if ((machine = malloc(sizeof(chip8))) == NULL) {
        printf("%s\n", strerror(errno));
        return NULL;
    }

    // initialize memory for the chip8 object
    memset(machine, '\0', sizeof(chip8)); 
    machine->pc = 0x200;
    /*
    memset(machine->mem, '\0', sizeof(machine->mem));
    memset(machine->V, '\0', sizeof(machine->V));
    memset(machine->disp, '\0', sizeof(machine->disp));
    memset(machine->stack, '\0', sizeof(machine->stack));
    memset(machine->key, '\0', sizeof(machine->key));
    machine->I = 0;
    machine->pc = 0x200;
    machine->dt= 0;
    machine->st= 0;
    machine->sp = 0;
    machine->opcode = 0;
    */

    // load the fontset into memory
    for (int i = 0; i < 80; i++) {
        machine->mem[i] = fontset[i];
    }

    // read the rom file into memory
    unsigned char *subbuff = &machine->mem[machine->pc];
    if (read(fd, subbuff, 4096) == -1) {
        printf("%s\n", strerror(errno));
        free(machine);
        return NULL;
    }

    // setup display
    display_init();
    render(machine);

    return machine;
}

int main(int argc, char *argv[]) {
    // validate arguments
    if (argc != 2) {
        printf("Usage: ./emu <rom>\n");
        exit(1);
    }
    char *rom = argv[1];

    chip8 *machine = initialize(rom);
    if (machine == NULL)
        exit(1);

    // cpu loop
    while (1) {
        // TODO: limit how fast this can run to some reasonable speed
        cycle(machine);
        if (check_for_exit())
            exit(EXIT_SUCCESS);
    }
    
    // should never get here
    return 0;
}
