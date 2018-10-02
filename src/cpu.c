#include "debug.h"
#include "keyboard.h"
#include "chip8.h"
#include "display.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <SDL2/SDL.h>

static void CLS(chip8 *machine);
static void RET(chip8 *machine);
static void JP_ADDR(chip8 *machine);
static void CALL_ADDR(chip8 *machine);
static void SE_REG_BYTE(chip8 *machine);
static void SNE_REG_BYTE(chip8 *machine);
static void SE_REG_REG(chip8 *machine);
static void LD_REG_BYTE(chip8 *machine);
static void ADD_REG_BYTE(chip8 *machine);
static void LD_REG_REG(chip8 *machine);
static void OR_REG_REG(chip8 *machine);
static void AND_REG_REG(chip8 *machine);
static void XOR_REG_REG(chip8 *machine);
static void ADD_REG_REG(chip8 *machine);
static void SUB_REG_REG(chip8 *machine);
static void SHR(chip8 *machine);
static void SUBN(chip8 *machine);
static void SHL(chip8 *machine);
static void SNE_REG_REG(chip8 *machine);
static void LD_I_ADDR(chip8 *machine);
static void JP_V0_ADDR(chip8 *machine);
static void RND(chip8 *machine);
static void DRW(chip8 *machine);
static void SKP(chip8 *machine);
static void SKNP(chip8 *machine);
static void LD_REG_DT(chip8 *machine);
static void LD_REG_K(chip8 *machine);
static void LD_DT_REG(chip8 *machine);
static void LD_ST_REG(chip8 *machine);
static void ADD_I_REG(chip8 *machine);
static void LD_F_REG(chip8 *machine);
static void LD_B_REG(chip8 *machine);
static void LD_I_REG(chip8 *machine);
static void LD_REG_I(chip8 *machine);
static unsigned short fetch_opcode(chip8 *machine);
static void decrement_timers(chip8 *machine);

// execute a single CPU cycle
void cycle(chip8 *machine) {
    machine->opcode = fetch_opcode(machine);
    set_keys(machine); 

    // decode and execute the opcode 
    // gross massive nested switch/case to map opcodes to functions
    switch (machine->opcode >> 12) {
        case 0x0: 
            switch (machine->opcode & 0xff) {
                case 0xe0: CLS(machine); break;
                case 0xee: RET(machine); break;
                default: debug("UNKNOWN INSTRUCTION 0\n"); break;
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
                default: debug("UNKNOWN INSTRUCTION 8\n"); break;
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
                default: debug("UNKNOWN INSTRUCTION E\n"); break;
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
                default: debug("UNKNOWN INSTRUCTION F\n"); break;
            }
            break;
        default: debug("UNKNOWN INSTRUCTION\n"); break;
    }

    // update the display
    if (machine->draw) {
        render(machine); 
        machine->draw = 0;
    }
    decrement_timers(machine); // update the timers
    machine->pc+=2; // update the program counter
}

// decrement the timers, to be called at the end of each CPU cycle
static void decrement_timers(chip8 *machine) {
    if (machine->dt> 0) {
        machine->dt--;
    }   

    if (machine->st> 0) {
        machine->st--;
        if (machine->st== 0) {
            beep();
        }   
    }   
}

// get the next unread opcode from memory
static unsigned short fetch_opcode(chip8 *machine) {
    unsigned short byte1 = machine->mem[machine->pc];
    unsigned short byte2 = machine->mem[machine->pc + 1];
    return (byte1 << 8) | byte2; 
}

// get the register Vx specified in the opcode
static unsigned char *Vx(chip8 *machine) {
    return &machine->V[(machine->opcode&0xf00)>>8];
}

// get the register Vy specified in the opcode
static unsigned char *Vy(chip8 *machine) {
    return &machine->V[(machine->opcode&0xf0)>>4];
}

// get the last byte in the opcode
static unsigned char byte(chip8 *machine) {
    return (machine->opcode)&0xff;
}

// get the last nibble in the opcode
static unsigned char nibble(chip8 *machine) {
    return (machine->opcode)&0xf;
}

// get the last 1.5 bytes in the opcode, representing a memory address in instructions 
static unsigned short addr(chip8 *machine) {
    return machine->opcode&0xfff;
}

// CLS: clear the display
static void CLS(chip8 *machine) { 
    debug("%03x CLS\n", machine->pc);
    memset(machine->disp, '\0', sizeof(machine->disp));
    machine->draw = 1;
}

// RET: set pc to address on top of stack, subtract one from sp
static void RET(chip8 *machine) { 
    debug("%03x RET\n", machine->pc);
    machine->pc = machine->stack[machine->sp--];
}

// JP addr: set pc to addr
static void JP_ADDR(chip8 *machine) { 
    debug("%03x JP_ADDR\n", machine->pc);
    machine->pc = addr(machine)-2;
}

// CALL addr: increment sp, push current pc to stack, set pc to addr
static void CALL_ADDR(chip8 *machine) {
    debug("%03x CALL_ADDR\n", machine->pc);
    machine->stack[++machine->sp] = machine->pc;
    machine->pc = addr(machine)-2;
}

// SE Vx, byte: if register Vx == byte, increment pc by 2
static void SE_REG_BYTE(chip8 *machine) { 
    debug("%03x SE_REG_BYTE\n", machine->pc);

    if (*Vx(machine) == byte(machine)) {
        machine->pc += 2;
    }
}

// SNE Vx, byte: if register Vx != byte, increment pc by 2
static void SNE_REG_BYTE(chip8 *machine) { 
    debug("%03x SNE_REG_BYTE\n", machine->pc);
    if (*Vx(machine) != byte(machine)) {
        machine->pc += 2;
    }
}

// SE Vx, Vy; if Vx == Vy, increment pc by 2
static void SE_REG_REG(chip8 *machine) { 
    debug("%03x SNE_REG_REG\n", machine->pc);
    if (*Vx(machine) == *Vy(machine)) {
        machine->pc += 2;
    }
}

// LD Vx, byte: store byte in Vx
static void LD_REG_BYTE(chip8 *machine) {
    debug("%03x LD_REG_BYTE\n", machine->pc);
    *Vx(machine) = byte(machine);
}

// ADD Vx, byte: add byte to Vx
static void ADD_REG_BYTE(chip8 *machine) { 
    debug("%03x ADD_REG_BYTE\n", machine->pc);
    *Vx(machine) += byte(machine);
}

// LD Vx, Vy: store Vy in Vx
static void LD_REG_REG(chip8 *machine) { 
    debug("%03x LD_REG_REG\n", machine->pc); 
    *Vx(machine) = *Vy(machine);
}

// OR Vx, Vy: Vx |= Vy
static void OR_REG_REG(chip8 *machine) { 
    debug("%03x OR_REG_REG\n", machine->pc); 
    *Vx(machine) |= *Vy(machine);
}

// AND Vx, Vy: Vx &= Vy
static void AND_REG_REG(chip8 *machine) { 
    debug("%03x AND_REG_REG\n", machine->pc); 
    *Vx(machine) &= *Vy(machine);
}

// XOR Vx, Vy: Vx ^= Vy
static void XOR_REG_REG(chip8 *machine) { 
    debug("%03x XOR_REG_REG\n", machine->pc); 
    *Vx(machine) ^= *Vy(machine);
}

// ADD Vx, Vy: Vx += Vy. If the result is more than 255 bits, set VF to 1
static void ADD_REG_REG(chip8 *machine) { 
    debug("%03x ADD_REG_REG\n", machine->pc); 
    unsigned int sum = *Vx(machine) + *Vy(machine);
    machine->V[0xf] = sum > 255 ? 1 : 0;
    *Vx(machine) = sum;
}

// SUB Vx, Vy: Vx -= Vy, set VF = NOT borrow
// if Vx > Vy, VF set to 1, otherwise 0, then subtraction occurs
static void SUB_REG_REG(chip8 *machine) { 
    debug("%03x SUB_REG_REG\n", machine->pc); 
    machine->V[0xf] = *Vx(machine) > *Vy(machine) ? 1 : 0;
    *Vx(machine) -= *Vy(machine);
}

// SHR VX {, Vy} If least significant bit of Vx is 1, VF is set to 1, otherwise 0. Vx is divided by 2
static void SHR(chip8 *machine) { 
    debug("%03x SHR\n", machine->pc); 
    machine->V[0xf] = (*Vx(machine) & 0x1) == 1 ? 1 : 0;
    *Vx(machine) /= 2;
}

// SUBN Vx, Vy: Vx = Vy - Vx, VF = NOT borrow
static void SUBN(chip8 *machine) { 
    debug("%03x SUBN\n", machine->pc); 
    machine->V[0xf] = *Vy(machine) > *Vx(machine) ? 1 : 0;
    *Vx(machine) = *Vy(machine) - *Vx(machine);
}

// SHL Vx {, Vy} (shift left). If most significant bit of Vx is 1, VF = 1, otherwise 0
static void SHL(chip8 *machine) { 
    debug("%03x SHL\n", machine->pc); 
    machine->V[0xf] = (*Vx(machine) & 0x1) == 1 ? 1 : 0;
    *Vx(machine) *= 2;
}

// SNE Vx, Vy: if Vx != Vy, increment pc by 2
static void SNE_REG_REG(chip8 *machine) {
    debug("%03x SNE_REG_REG\n", machine->pc);
    if (*Vx(machine) == *Vy(machine)) {
        machine->pc += 2;
    }
}

// LD I, addr: set I to addr
static void LD_I_ADDR(chip8 *machine) { 
    debug("%03x LD_I_ADDR\n", machine->pc); 
    machine->I = addr(machine);
}

// JP V0, addr: set pc to V0 + addr
static void JP_V0_ADDR(chip8 *machine) { 
    debug("%03x JP_V0_ADDR\n", machine->pc); 
    machine->pc = machine->V[0] + addr(machine)-2;
}

// RND Vx, byte:generate random number from 0-255, AND with byte, store in Vx
static void RND(chip8 *machine) { 
    debug("%03x RND\n", machine->pc); 
    *Vx(machine) = (rand() % 256) & byte(machine);
}

// DRW Vx, Vy, nibble: Draw nibble-bit sprite at memory location I starting at (Vx, Vy),
// set VF = collision
static void DRW(chip8 *machine) { 
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
    machine->draw = 1;
}

// SKP Vx: Skip the next instruction if key Vx is pressed
static void SKP(chip8 *machine) { 
    debug("%03x SKP\n", machine->pc); 
    if (machine->key[*Vx(machine)] == 1)
        machine->pc += 2;
}

// SKNP Vx: Skip the next instruction if key Vx is not pressed
static void SKNP(chip8 *machine) { 
    debug("%03x SKNP\n", machine->pc); 
    if (machine->key[*Vx(machine)] != 1)
        machine->pc += 2;
}

// LD Vx, DT: set Vx to DT
static void LD_REG_DT(chip8 *machine) { 
    debug("%03x LD_REG_DT\n", machine->pc);
    *Vx(machine) = machine->dt;
}

// LD Vx, K: wait for a key press, store value in Vx
static void LD_REG_K(chip8 *machine) { 
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

static void LD_DT_REG(chip8 *machine) { 
    debug("%03x LD_DT_REG\n", machine->pc); 
    machine->dt = *Vx(machine);
}

// LD ST, Vx: set ST to Vx
static void LD_ST_REG(chip8 *machine) { 
    debug("%03x LD_ST_REG\n", machine->pc);
    machine->st = *Vx(machine);
}

// ADD I, Vx: I += Vx
static void ADD_I_REG(chip8 *machine) { 
    debug("%03x ADD_I_REG\n", machine->pc); 
    machine->I += *Vx(machine);
}

// LD F,Vx: Set I = location of sprite for the digit Vx
static void LD_F_REG(chip8 *machine) { 
    debug("%03x LD_F_REG\n", machine->pc); 
    machine->I = *Vx(machine) * 5;
}

// LD B, Vx: Store BCD representation of Vx at I->I+2
static void LD_B_REG(chip8 *machine) { 
    debug("%03x LD_B_REG\n", machine->pc); 
    int value = *Vx(machine);
    machine->mem[machine->I] = (value % 1000) / 100;
    machine->mem[machine->I+1] = (value % 100) / 10;
    machine->mem[machine->I+2] = (value % 10);
}

// LD I, Vx: store registers v0 to Vx in memory starting at I
static void LD_I_REG(chip8 *machine) { 
    debug("%03x LD_I_REG\n", machine->pc); 
    for (int i = 0; i <= (machine->opcode&0xf00)>>8; i++) {
        machine->mem[machine->I + i] = machine->V[i];
    }
}

// LD Vx, I: read V0 to Vx from memory, starting at I
static void LD_REG_I(chip8 *machine) { 
    debug("%03x LD_REG_I\n", machine->pc); 
    for (unsigned char i = 0; i <= (machine->opcode&0xf00)>>8; i++) {
        machine->V[i] = machine->mem[machine->I + i];
    }
}
