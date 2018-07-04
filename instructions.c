#include "instructions.h"
#include <stdio.h>
#include <string.h>

// CLS: clear the display
void CLS(chip8 *machine) { 
    memset(machine->disp, '\0', sizeof(machine->disp));
}

// RET: set pc to address on top of stack, subtract one from sp
void RET(chip8 *machine) { 
    machine->pc = machine->stack[machine->sp--];
}

// JP addr: set pc to addr
void JP_ADDR(chip8 *machine) { 
    machine->pc = machine->opcode&0xfff;
}

// CALL addr: increment sp, push current pc to stack, set pc to addr
void CALL_ADDR(chip8 *machine) {
    machine->stack[++machine->sp] = machine->pc;
    machine->pc = machine->opcode&0xfff;
}

// SE Vx, byte: if register Vx == byte, increment pc by 2
void SE_REG_BYTE(chip8 *machine) { 
    if (machine->V[(machine->opcode&0xf00)>>2] == (machine->opcode&0xff)) {
        machine->pc += 2;
    }
}

// SNE Vx, byte: if register Vx != byte, increment pc by 2
void SNE_REG_BYTE(chip8 *machine) { 
    if (machine->V[(machine->opcode&0xf00)>>2] != (machine->opcode&0xff)) {
        machine->pc += 2;
    }
}

// SE Vx, Vy; if Vx == Vy, increment pc by 2
void SE_REG_REG(chip8 *machine) { 
    if (machine->V[(machine->opcode&0xf00)>>2] == machine->V[machine->opcode&0xf0>>1]) {
        machine->pc += 2;
    }
}

// LD Vx, byte: store byte in Vx
void LD_REG_BYTE(chip8 *machine) {
    machine->V[(machine->opcode&0xf00)>>2] = machine->opcode&0xff;
}


void ADD_REG_BYTE(chip8 *machine) { printf("ADD_REG_BYTE\n"); }
void LD_REG_REG(chip8 *machine) { printf("LD_REG_REG\n"); }
void OR_REG_REG(chip8 *machine) { printf("OR_REG_REG\n"); }
void AND_REG_REG(chip8 *machine) { printf("AND_REG_REG\n"); }
void XOR_REG_REG(chip8 *machine) { printf("XOR_REG_REG\n"); }
void ADD_REG_REG(chip8 *machine) { printf("ADD_REG_REG\n"); }
void SUB_REG_REG(chip8 *machine) { printf("SUB_REG_REG\n"); }
void SHR(chip8 *machine) { printf("SHR\n"); }
void SUBN(chip8 *machine) { printf("SUBN\n"); }
void SHL(chip8 *machine) { printf("SHL\n"); }

// SNE Vx, Vy: if Vx != Vy, increment pc by 2
void SNE_REG_REG(chip8 *machine) {
    if (machine->V[(machine->opcode&0xf00)>>2] == machine->V[machine->opcode&0xf0>>1]) {
        machine->pc += 2;
    }
}

void LD_I_ADDR(chip8 *machine) { printf("LD_I_ADDR\n"); }
void JP_V0_ADDR(chip8 *machine) { printf("JP_V0_ADDR\n"); }
void RND(chip8 *machine) { printf("RND\n"); }
void DRW(chip8 *machine) { printf("DRW\n"); }
void SKP(chip8 *machine) { printf("SKP\n"); }
void SKNP(chip8 *machine) { printf("SKNP\n"); }
void LD_REG_DT(chip8 *machine) { printf("LD_REG_DT\n"); }
void LD_REG_K(chip8 *machine) { printf("LD_REG_K\n"); }
void LD_DT_REG(chip8 *machine) { printf("LD_DT_REG\n"); }
void LD_ST_REG(chip8 *machine) { printf("LD_ST_REG\n"); }
void ADD_I_REG(chip8 *machine) { printf("ADD_I_REG\n"); }
void LD_F_REG(chip8 *machine) { printf("LD_F_REG\n"); }
void LD_B_REG(chip8 *machine) { printf("LD_B_REG\n"); }
void LD_I_REG(chip8 *machine) { printf("LD_I_REG\n"); }
void LD_REG_I(chip8 *machine) { printf("LD_REG_I\n"); }
