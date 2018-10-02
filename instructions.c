#include "instructions.h"
#include "debug.h"
#include "keyboard.h"
#include "config.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <SDL2/SDL.h>

static unsigned char *Vx(chip8 *machine) {
    return &machine->V[(machine->opcode&0xf00)>>8];
}

static unsigned char *Vy(chip8 *machine) {
    return &machine->V[(machine->opcode&0xf0)>>4];
}

static unsigned char byte(chip8 *machine) {
    return (machine->opcode)&0xff;
}

static unsigned char nibble(chip8 *machine) {
    return (machine->opcode)&0xf;
}

static unsigned short addr(chip8 *machine) {
    return machine->opcode&0xfff;
}

// CLS: clear the display
void CLS(chip8 *machine) { 
    debug("%03x CLS\n", machine->pc);
    memset(machine->disp, '\0', sizeof(machine->disp));
}

// RET: set pc to address on top of stack, subtract one from sp
void RET(chip8 *machine) { 
    debug("%03x RET\n", machine->pc);
    machine->pc = machine->stack[machine->sp--];
}

// JP addr: set pc to addr
void JP_ADDR(chip8 *machine) { 
    debug("%03x JP_ADDR\n", machine->pc);
    machine->pc = addr(machine)-2;
}

// CALL addr: increment sp, push current pc to stack, set pc to addr
void CALL_ADDR(chip8 *machine) {
    debug("%03x CALL_ADDR\n", machine->pc);
    machine->stack[++machine->sp] = machine->pc;
    machine->pc = addr(machine)-2;
}

// SE Vx, byte: if register Vx == byte, increment pc by 2
void SE_REG_BYTE(chip8 *machine) { 
    debug("%03x SE_REG_BYTE\n", machine->pc);

    if (*Vx(machine) == byte(machine)) {
        machine->pc += 2;
    }
}

// SNE Vx, byte: if register Vx != byte, increment pc by 2
void SNE_REG_BYTE(chip8 *machine) { 
    debug("%03x SNE_REG_BYTE\n", machine->pc);
    if (*Vx(machine) != byte(machine)) {
        machine->pc += 2;
    }
}

// SE Vx, Vy; if Vx == Vy, increment pc by 2
void SE_REG_REG(chip8 *machine) { 
    debug("%03x SNE_REG_REG\n", machine->pc);
    if (*Vx(machine) == *Vy(machine)) {
        machine->pc += 2;
    }
}

// LD Vx, byte: store byte in Vx
void LD_REG_BYTE(chip8 *machine) {
    debug("%03x LD_REG_BYTE\n", machine->pc);
    *Vx(machine) = byte(machine);
}

// ADD Vx, byte: add byte to Vx
void ADD_REG_BYTE(chip8 *machine) { 
    debug("%03x ADD_REG_BYTE\n", machine->pc);
    *Vx(machine) += byte(machine);
}

// LD Vx, Vy: store Vy in Vx
void LD_REG_REG(chip8 *machine) { 
    debug("%03x LD_REG_REG\n", machine->pc); 
    *Vx(machine) = *Vy(machine);
}

// OR Vx, Vy: Vx |= Vy
void OR_REG_REG(chip8 *machine) { 
    debug("%03x OR_REG_REG\n", machine->pc); 
    *Vx(machine) |= *Vy(machine);
}

// AND Vx, Vy: Vx &= Vy
void AND_REG_REG(chip8 *machine) { 
    debug("%03x AND_REG_REG\n", machine->pc); 
    *Vx(machine) &= *Vy(machine);
}

// XOR Vx, Vy: Vx ^= Vy
void XOR_REG_REG(chip8 *machine) { 
    debug("%03x XOR_REG_REG\n", machine->pc); 
    *Vx(machine) ^= *Vy(machine);
}

// ADD Vx, Vy: Vx += Vy. If the result is more than 255 bits, set VF to 1
void ADD_REG_REG(chip8 *machine) { 
    debug("%03x ADD_REG_REG\n", machine->pc); 
    unsigned int sum = *Vx(machine) + *Vy(machine);
    machine->V[0xf] = sum > 255 ? 1 : 0;
    *Vx(machine) = sum;
}

// SUB Vx, Vy: Vx -= Vy, set VF = NOT borrow
// if Vx > Vy, VF set to 1, otherwise 0, then subtraction occurs
void SUB_REG_REG(chip8 *machine) { 
    debug("%03x SUB_REG_REG\n", machine->pc); 
    machine->V[0xf] = *Vx(machine) > *Vy(machine) ? 1 : 0;
    *Vx(machine) -= *Vy(machine);
}

// SHR VX {, Vy} If least significant bit of Vx is 1, VF is set to 1, otherwise 0. Vx is divided by 2
void SHR(chip8 *machine) { 
    debug("%03x SHR\n", machine->pc); 
    machine->V[0xf] = (*Vx(machine) & 0x1) == 1 ? 1 : 0;
    *Vx(machine) /= 2;
}

// SUBN Vx, Vy: Vx = Vy - Vx, VF = NOT borrow
void SUBN(chip8 *machine) { 
    debug("%03x SUBN\n", machine->pc); 
    machine->V[0xf] = *Vy(machine) > *Vx(machine) ? 1 : 0;
    *Vx(machine) = *Vy(machine) - *Vx(machine);
}

// SHL Vx {, Vy} (shift left). If most significant bit of Vx is 1, VF = 1, otherwise 0
void SHL(chip8 *machine) { 
    debug("%03x SHL\n", machine->pc); 
    machine->V[0xf] = (*Vx(machine) & 0x1) == 1 ? 1 : 0;
    *Vx(machine) *= 2;
}

// SNE Vx, Vy: if Vx != Vy, increment pc by 2
void SNE_REG_REG(chip8 *machine) {
    debug("%03x SNE_REG_REG\n", machine->pc);
    if (*Vx(machine) == *Vy(machine)) {
        machine->pc += 2;
    }
}

// LD I, addr: set I to addr
void LD_I_ADDR(chip8 *machine) { 
    debug("%03x LD_I_ADDR\n", machine->pc); 
    machine->I = addr(machine);
}

// JP V0, addr: set pc to V0 + addr
void JP_V0_ADDR(chip8 *machine) { 
    debug("%03x JP_V0_ADDR\n", machine->pc); 
    machine->pc = machine->V[0] + addr(machine)-2;
}

// RND Vx, byte:generate random number from 0-255, AND with byte, store in Vx
void RND(chip8 *machine) { 
    debug("%03x RND\n", machine->pc); 
    *Vx(machine) = (rand() % 256) & byte(machine);
}

// DRW Vx, Vy, nibble: Draw nibble-bit sprite at memory location I starting at (Vx, Vy),
// set VF = collision
void DRW(chip8 *machine) { 
    debug("%03x DRW\n", machine->pc); 
    machine->V[0xf] = 0;

    int startX = *Vx(machine);
    int startY = *Vy(machine);

    unsigned int height = nibble(machine);
    for (int y = 0; y < height; y++) {
        unsigned char value = machine->mem[machine->I + y];
        for (int x = 0; x < 8; x++) {
            unsigned char bit = value & (0x80 >> x); // the xth bit in the byte
            int b = (bit) ? 1 : 0;
            int coord = (startY + y)*CHIP8_WIDTH + (startX + x);
            if (b) {
                if (machine->disp[coord]) {
                    machine->V[0xf] = 1; // collision
                    machine->disp[coord] = 0;
                } else {
                    machine->disp[coord] = 1;
                }
            }
        }
    }
}

// SKP Vx: Skip the next instruction if key Vx is pressed
void SKP(chip8 *machine) { 
    debug("%03x SKP\n", machine->pc); 
    if (machine->key[*Vx(machine)] == 1)
        machine->pc += 2;
}

// SKNP Vx: Skip the next instruction if key Vx is not pressed
void SKNP(chip8 *machine) { 
    debug("%03x SKNP\n", machine->pc); 
    if (machine->key[*Vx(machine)] != 1)
        machine->pc += 2;
}

// LD Vx, DT: set Vx to DT
void LD_REG_DT(chip8 *machine) { 
    debug("%03x LD_REG_DT\n", machine->pc);
    *Vx(machine) = machine->dt;
}

// LD Vx, K: wait for a key press, store value in Vx
void LD_REG_K(chip8 *machine) { 
    debug("%03x LD_REG_K\n", machine->pc); 
    int keyEvent = 0;
    SDL_Event event;
    while (!keyEvent) {
        SDL_WaitEvent(&event);
        if (event.type == SDL_KEYDOWN) {
            check_for_exit();
            int scancode = event.key.keysym.scancode;
            if (scancode == SDL_SCANCODE_ESCAPE)
                exit(0);
            if (key_from_sdlscancode(scancode) != -1) {
                keyEvent = 1;
                *Vx(machine) = key_from_sdlscancode(scancode);
            }
        }
    }
}


// LD DT, Vx: set DT to Vx

void LD_DT_REG(chip8 *machine) { 
    debug("%03x LD_DT_REG\n", machine->pc); 
    machine->dt = *Vx(machine);
}

// LD ST, Vx: set ST to Vx
void LD_ST_REG(chip8 *machine) { 
    debug("%03x LD_ST_REG\n", machine->pc);
    machine->st = *Vx(machine);
}

// ADD I, Vx: I += Vx
void ADD_I_REG(chip8 *machine) { 
    debug("%03x ADD_I_REG\n", machine->pc); 
    machine->I += *Vx(machine);
}

// LD F,Vx: Set I = location of sprite for the digit Vx
void LD_F_REG(chip8 *machine) { 
    debug("%03x LD_F_REG\n", machine->pc); 
    machine->I = *Vx(machine) * 5;
}

// LD B, Vx: Store BCD representation of Vx at I->I+2
void LD_B_REG(chip8 *machine) { 
    debug("%03x LD_B_REG\n", machine->pc); 
    int value = *Vx(machine);
    machine->mem[machine->I] = (value % 1000) / 100;
    machine->mem[machine->I+1] = (value % 100) / 10;
    machine->mem[machine->I+2] = (value % 10);
}

// LD I, Vx: store registers v0 to Vx in memory starting at I
void LD_I_REG(chip8 *machine) { 
    debug("%03x LD_I_REG\n", machine->pc); 
    for (int i = 0; i <= (machine->opcode&0xf00)>>8; i++) {
        machine->mem[machine->I + i] = machine->V[i];
    }
}

// LD Vx, I: read V0 to Vx from memory, starting at I
void LD_REG_I(chip8 *machine) { 
    debug("%03x LD_REG_I\n", machine->pc); 
    for (unsigned char i = 0; i <= (machine->opcode&0xf00)>>8; i++) {
        machine->V[i] = machine->mem[machine->I + i];
    }
}
