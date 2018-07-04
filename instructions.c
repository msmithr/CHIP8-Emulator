#include "instructions.h"
#include "debug.h"
#include <stdio.h>
#include <string.h>

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
    machine->pc = machine->opcode&0xfff;
}

// CALL addr: increment sp, push current pc to stack, set pc to addr
void CALL_ADDR(chip8 *machine) {
    debug("%03x CALL_ADDR\n", machine->pc);
    machine->stack[++machine->sp] = machine->pc;
    machine->pc = machine->opcode&0xfff;
}

// SE Vx, byte: if register Vx == byte, increment pc by 2
void SE_REG_BYTE(chip8 *machine) { 
    debug("%03x SE_REG_BYTE\n", machine->pc);
    if (machine->V[(machine->opcode&0xf00)>>2] == (machine->opcode&0xff)) {
        machine->pc += 2;
    }
}

// SNE Vx, byte: if register Vx != byte, increment pc by 2
void SNE_REG_BYTE(chip8 *machine) { 
    debug("%03x SNE_REG_BYTE\n", machine->pc);
    if (machine->V[(machine->opcode&0xf00)>>2] != (machine->opcode&0xff)) {
        machine->pc += 2;
    }
}

// SE Vx, Vy; if Vx == Vy, increment pc by 2
void SE_REG_REG(chip8 *machine) { 
    debug("%03x SNE_REG_REG\n", machine->pc);
    if (machine->V[(machine->opcode&0xf00)>>2] == machine->V[machine->opcode&0xf0>>1]) {
        machine->pc += 2;
    }
}

// LD Vx, byte: store byte in Vx
void LD_REG_BYTE(chip8 *machine) {
    debug("%03x LD_REG_BYTE\n", machine->pc);
    machine->V[(machine->opcode&0xf00)>>2] = machine->opcode&0xff;
}

// ADD Vx, byte: add byte to Vx
void ADD_REG_BYTE(chip8 *machine) { 
    debug("%03x ADD_REG_BYTE\n", machine->pc);
    machine->V[(machine->opcode&0xf00)>>2] += machine->opcode&0xff;
}

// LD Vx, Vy: store Vy in Vx
void LD_REG_REG(chip8 *machine) { 
    debug("%03x LD_REG_REG\n", machine->pc); 
    machine->V[(machine->opcode&0xf00)>>2] = machine->V[(machine->opcode&0xf0)>>1];
}

// OR Vx, Vy: Vx |= Vy
void OR_REG_REG(chip8 *machine) { 
    debug("%03x OR_REG_REG\n", machine->pc); 
    machine->V[(machine->opcode&0xf00)>>2] |= machine->V[(machine->opcode&0xf0)>>1];
}

// AND Vx, Vy: Vx &= Vy
void AND_REG_REG(chip8 *machine) { 
    debug("%03x AND_REG_REG\n", machine->pc); 
    machine->V[(machine->opcode&0xf00)>>2] &= machine->V[(machine->opcode&0xf0)>>1];
}

// XOR Vx, Vy: Vx ^= Vy
void XOR_REG_REG(chip8 *machine) { 
    debug("%03x XOR_REG_REG\n", machine->pc); 
    machine->V[(machine->opcode&0xf00)>>2] ^= machine->V[(machine->opcode&0xf0)>>1];
}

void ADD_REG_REG(chip8 *machine) { debug("%03x ADD_REG_REG\n", machine->pc); }
void SUB_REG_REG(chip8 *machine) { debug("%03x SUB_REG_REG\n", machine->pc); }
void SHR(chip8 *machine) { debug("%03x SHR\n", machine->pc); }
void SUBN(chip8 *machine) { debug("%03x SUBN\n", machine->pc); }
void SHL(chip8 *machine) { debug("%03x SHL\n", machine->pc); }

// SNE Vx, Vy: if Vx != Vy, increment pc by 2
void SNE_REG_REG(chip8 *machine) {
    debug("%03x SNE_REG_REG\n", machine->pc);
    if (machine->V[(machine->opcode&0xf00)>>2] == machine->V[machine->opcode&0xf0>>1]) {
        machine->pc += 2;
    }
}

void LD_I_ADDR(chip8 *machine) { debug("%03x LD_I_ADDR\n", machine->pc); }
void JP_V0_ADDR(chip8 *machine) { debug("%03x JP_V0_ADDR\n", machine->pc); }
void RND(chip8 *machine) { debug("%03x RND\n", machine->pc); }
void DRW(chip8 *machine) { debug("%03x DRW\n", machine->pc); }
void SKP(chip8 *machine) { debug("%03x SKP\n", machine->pc); }
void SKNP(chip8 *machine) { debug("%03x SKNP\n", machine->pc); }
void LD_REG_DT(chip8 *machine) { debug("%03x LD_REG_DT\n", machine->pc); }
void LD_REG_K(chip8 *machine) { debug("%03x LD_REG_K\n", machine->pc); }
void LD_DT_REG(chip8 *machine) { debug("%03x LD_DT_REG\n", machine->pc); }
void LD_ST_REG(chip8 *machine) { debug("%03x LD_ST_REG\n", machine->pc); }
void ADD_I_REG(chip8 *machine) { debug("%03x ADD_I_REG\n", machine->pc); }
void LD_F_REG(chip8 *machine) { debug("%03x LD_F_REG\n", machine->pc); }
void LD_B_REG(chip8 *machine) { debug("%03x LD_B_REG\n", machine->pc); }
void LD_I_REG(chip8 *machine) { debug("%03x LD_I_REG\n", machine->pc); }
void LD_REG_I(chip8 *machine) { debug("%03x LD_REG_I\n", machine->pc); }
