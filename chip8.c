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

chip8 *initialize(char *rom) {
    int fd;
    chip8 *machine;

    if ((fd = open(rom, O_RDONLY)) == -1) {
        printf("%s\n", strerror(errno));
        return NULL;
    }

    if ((machine = malloc(sizeof(chip8))) == NULL) {
        printf("%s\n", strerror(errno));
        return NULL;
    }

    memset(machine->mem, '\0', sizeof(machine->mem));
    memset(machine->V, '\0', sizeof(machine->V));
    memset(machine->disp, '\0', sizeof(machine->disp));
    memset(machine->stack, '\0', sizeof(machine->stack));
    memset(machine->key, '\0', sizeof(machine->key));
    machine->I = 0;
    machine->pc = 0x200;
    machine->delay_timer = 0;
    machine->sound_timer = 0;
    machine->sp = 0;
    machine->opcode = 0;

    unsigned char *subbuff = &machine->mem[machine->pc];
    if (read(fd, subbuff, 4096) == -1) {
        printf("%s\n", strerror(errno));
        free(machine);
        return NULL;
    }

    return machine;
}

static unsigned short fetch_opcode(chip8 *machine) {
    unsigned short byte1 = machine->mem[machine->pc];
    unsigned short byte2 = machine->mem[machine->pc + 1];
    return (byte1 << 8) | byte2; 
}

static void decrement_timers(chip8 *machine) {
    if (machine->delay_timer > 0) {
        machine->delay_timer--;
    }   

    if (machine->sound_timer > 0) {
        machine->sound_timer--;
        if (machine->sound_timer == 0) {
            printf("BEEP\n");
        }   
    }   
}


void cycle(chip8 *machine) {
    unsigned short opcode = fetch_opcode(machine);

    /* decode and execute the opcode*/
    switch (opcode >> 12) {
        case 0x0: 
            switch (opcode & 0xf) {
                case 0xe0: CLS(machine); break;
                case 0xee: RET(machine); break;
                default: printf("UNKNOWN INSTRUCTION\n"); break;
            }
        case 0x1: JP_ADDR(machine); break;
        case 0x2: CALL_ADDR(machine); break;
        case 0x3: SE_REG_BYTE(machine); break;
        case 0x4: SNE_REG_BYTE(machine); break;
        case 0x5: SE_REG_REG(machine); break;
        case 0x6: LD_REG_BYTE(machine); break;
        case 0x7: ADD_REG_BYTE(machine); break;
        case 0x8: 
            switch (opcode & 0x3) {
                case 0x0: LD_REG_REG(machine); break;
                case 0x1: OR_REG_REG(machine); break;
                case 0x2: AND_REG_REG(machine); break;
                case 0x3: XOR_REG_REG(machine); break;
                case 0x4: ADD_REG_REG(machine); break;
                case 0x5: SUB_REG_REG(machine); break;
                case 0x6: SHR(machine); break;
                case 0x7: SUBN(machine); break;
                case 0xe: SHL(machine); break;
                default: printf("UNKNOWN INSTRUCTION\n"); break;
            }
        case 0x9: SNE_REG_REG(machine); break;
        case 0xa: LD_I_ADDR(machine); break;
        case 0xb: JP_V0_ADDR(machine); break;
        case 0xc: RND(machine); break;
        case 0xd: DRW(machine); break;
        case 0xe: 
            switch (opcode & 0xf) {
                case 0x9e: SKP(machine); break;
                case 0xa1: SKNP(machine); break;
                default: printf("UNKNOWN INSTRUCTION\n"); break;
            }
        case 0xf: 
            switch (opcode & 0xf) {
                case 0x07: LD_REG_DT(machine); break;
                case 0x0a: LD_REG_K(machine); break;
                case 0x15: LD_DT_REG(machine); break;
                case 0x18: LD_ST_REG(machine); break;
                case 0x1e: ADD_I_REG(machine); break;
                case 0x29: LD_F_REG(machine); break;
                case 0x33: LD_B_REG(machine); break;
                case 0x55: LD_I_REG(machine); break;
                case 0x65: LD_REG_I(machine); break;
                default: printf("UNKNOWN INSTRUCTION\n"); break;
            }
        default: printf("UNKNOWN INSTRUCTION\n"); break;
    }

    decrement_timers(machine);

    machine->pc+=2;
}


int main() {
    chip8 *machine = initialize("pong.rom");

    printf("%d\n", machine->sound_timer);
    return 0;
}
