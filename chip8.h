#ifndef c8
#define c8
typedef struct c8 {
    unsigned char mem[4096];
    unsigned char V[16]; 
    unsigned short I;
    unsigned short pc; 
    unsigned char disp[64*32];
    unsigned char dt;
    unsigned char st;
    unsigned short stack[16];
    unsigned short sp; 
    unsigned char key[16];
    unsigned short opcode;
} chip8;
#endif

chip8 *initialize(char *rom);
