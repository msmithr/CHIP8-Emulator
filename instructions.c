#include "instructions.h"
#include <stdio.h>

void CLS(chip8 *machine) { printf("CLS\n"); }
void RET(chip8 *machine) { printf("RET\n"); }
void JP_ADDR(chip8 *machine) { printf("JP_ADDR\n"); }
void CALL_ADDR(chip8 *machine) { printf("CALL_ADDR\n"); }
void SE_REG_BYTE(chip8 *machine) { printf("SE_REG_BYTE\n"); }
void SNE_REG_BYTE(chip8 *machine) { printf("SNE_REG_BYTE\n"); }
void SE_REG_REG(chip8 *machine) { printf("SE_REG_REG\n"); }
void LD_REG_BYTE(chip8 *machine) { printf("LD_REG_BYTE\n"); }
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
void SNE_REG_REG(chip8 *machine) { printf("SNE_REG_REG\n"); }
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
