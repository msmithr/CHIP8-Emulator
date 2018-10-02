#ifndef c8
#define c8
typedef struct c8 {
    unsigned char mem[4096];    // memory
    unsigned char V[16];        // registers
    unsigned short I;           // memory location register
    unsigned short pc;          // program counter
    unsigned char disp[64*32];  // display state: 64*32 pixels, 0=black, 1=white
    unsigned char dt;           // delay timer
    unsigned char st;           // sound timer
    unsigned short stack[16];   // stack
    unsigned short sp;          // stack pointer
    unsigned char key[16];      // keypad state
    unsigned short opcode;      // the next opcode in memory
    unsigned short draw;        // flag determining whether a re-render is needed
} chip8;
#endif
