#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include "chip8.h"
#include "instructions.h"

static unsigned short fetch_opcode(chip8 *machine);
static void decrement_timers(chip8 *machine);
void cycle(chip8 *machine);

// initialize a chip8 object, given the filepath to a rom file
chip8 *initialize(char *rom) {
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

    // read the rom file into memory
    unsigned char *subbuff = &machine->mem[machine->pc];
    if (read(fd, subbuff, 4096) == -1) {
        printf("%s\n", strerror(errno));
        free(machine);
        return NULL;
    }

    return machine;
}

// get the next unread opcode from memory
static unsigned short fetch_opcode(chip8 *machine) {
    unsigned short byte1 = machine->mem[machine->pc];
    unsigned short byte2 = machine->mem[machine->pc + 1];
    return (byte1 << 8) | byte2; 
}

// decrement the timers, to be called at the end of each CPU cycle
static void decrement_timers(chip8 *machine) {
    if (machine->dt> 0) {
        machine->dt--;
    }   

    if (machine->st> 0) {
        machine->st--;
        if (machine->st== 0) {
            printf("BEEP\n");
        }   
    }   
}


// execute a single CPU cycle
void cycle(chip8 *machine) {
   machine->opcode = fetch_opcode(machine);

    /* decode and execute the opcode */
    switch (machine->opcode >> 12) {
        case 0x0: 
            switch (machine->opcode & 0xff) {
                case 0xe0: CLS(machine); break;
                case 0xee: RET(machine); break;
                default: printf("UNKNOWN INSTRUCTION 0\n"); break;
            }
            break;
        case 0x1: JP_ADDR(machine); break;
        case 0x2: CALL_ADDR(machine); break;
        case 0x3: SE_REG_BYTE(machine); break;
        case 0x4: SNE_REG_BYTE(machine); break;
        case 0x5: SE_REG_REG(machine); break;
        case 0x6: LD_REG_BYTE(machine); break;
        case 0x7: ADD_REG_BYTE(machine); break;
        case 0x8: 
            switch (machine->opcode & 0xf) {
                case 0x0: LD_REG_REG(machine); break;
                case 0x1: OR_REG_REG(machine); break;
                case 0x2: AND_REG_REG(machine); break;
                case 0x3: XOR_REG_REG(machine); break;
                case 0x4: ADD_REG_REG(machine); break;
                case 0x5: SUB_REG_REG(machine); break;
                case 0x6: SHR(machine); break;
                case 0x7: SUBN(machine); break;
                case 0xe: SHL(machine); break;
                default: printf("UNKNOWN INSTRUCTION 8\n"); break;
            }
            break;
        case 0x9: SNE_REG_REG(machine); break;
        case 0xa: LD_I_ADDR(machine); break;
        case 0xb: JP_V0_ADDR(machine); break;
        case 0xc: RND(machine); break;
        case 0xd: DRW(machine); break;
        case 0xe: 
            switch (machine->opcode & 0xff) {
                case 0x9e: SKP(machine); break;
                case 0xa1: SKNP(machine); break;
                default: printf("UNKNOWN INSTRUCTION E\n"); break;
            }
            break;
        case 0xf: 
            switch (machine->opcode & 0xff) {
                case 0x07: LD_REG_DT(machine); break;
                case 0x0a: LD_REG_K(machine); break;
                case 0x15: LD_DT_REG(machine); break;
                case 0x18: LD_ST_REG(machine); break;
                case 0x1e: ADD_I_REG(machine); break;
                case 0x29: LD_F_REG(machine); break;
                case 0x33: LD_B_REG(machine); break;
                case 0x55: LD_I_REG(machine); break;
                case 0x65: LD_REG_I(machine); break;
                default: printf("UNKNOWN INSTRUCTION F\n"); break;
            }
            break;
        default: printf("UNKNOWN INSTRUCTION\n"); break;
    }

    decrement_timers(machine);

    machine->pc+=2;
}


int main() {
    chip8 *machine = initialize("pong.rom");
    while (1) {
        cycle(machine);
    }

    return 0;
}
