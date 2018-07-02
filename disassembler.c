#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <stdint.h>

// print the instruction defined in the buffer at the program counter
// returns the program counter at the next instruction
int disassemble(unsigned char *buff, unsigned short pc);

int main(int argc, char *argv[]) {
    unsigned short pc = 0;
    unsigned char buff[4096];
    int fd;
    int nread;

    // argument validation
    if (argc != 2) {
        printf("Usage: ./dis <filename>\n");
        return EXIT_FAILURE;
    }

    // open the target file
    if ((fd = open(argv[1], O_RDONLY)) == -1) {
        printf("%s: %s\n", argv[0], strerror(errno));
        return EXIT_FAILURE;
    }

    // read the target file into a buffer
    if ((nread = read(fd, buff, 4096)) == -1) {
        printf("%s: %s\n", argv[0], strerror(errno));
        return EXIT_FAILURE;
    }

    // disassemble the file
    while ((pc = disassemble(buff, pc)) < nread);

    return 0;
}

int disassemble(unsigned char *buff, unsigned short pc) {
    uint8_t *opcode = &buff[pc];

    printf("%04x %02x %02x\t", pc, opcode[0], opcode[1]);
    switch (opcode[0]>>4) {
        case 0x0: 
            switch (opcode[1]) {
                case 0xe0: printf("CLS\n"); break;
                case 0xee: printf("RET\n"); break;
                default: printf("UNKNOWN INSTRUCTION\n"); break;
            }
            break;
        case 0x1: printf("JP $%01x%02x\n", opcode[0]&0xf, opcode[1]); break;
        case 0x2: printf("CALL $%01x%02x\n", opcode[0]&0xf, opcode[1]); break;
        case 0x3: printf("SE V%01x, #%02x\n", opcode[0]&0xf, opcode[1]); break;
        case 0x4: printf("SNE V%01x, #%02x\n", opcode[0]&0xf, opcode[1]); break;
        case 0x5: printf("SE V%01x, V%01x\n", opcode[0]&0xf, opcode[1]>>4); break;
        case 0x6: printf("LD V%01x, #%02x\n", opcode[0]&0xf, opcode[1]); break;
        case 0x7: printf("ADD V%01x, #%02x\n", opcode[0]&0xf, opcode[1]); break;
        case 0x8: 
            switch (opcode[1]&0x3) {
                case 0x0: printf("LD V%01x, V%01x\n", opcode[0]&0xf, opcode[1]>>4); break;
                case 0x1: printf("OR V%01x, V%01x\n", opcode[0]&0xf, opcode[1]>>4); break;
                case 0x2: printf("AND V%01x, V%01x\n", opcode[0]&0xf, opcode[1]>>4); break;
                case 0x3: printf("XOR V%01x, V%01x\n", opcode[0]&0xf, opcode[1]>>4); break;
                case 0x4: printf("ADD V%01x, V%01x\n", opcode[0]&0xf, opcode[1]>>4); break;
                case 0x5: printf("SUB V%01x, V%01x\n", opcode[0]&0xf, opcode[1]>>4); break;
                case 0x6: printf("SHR V%01x, V%01x\n", opcode[0]&0xf, opcode[1]>>4); break;
                case 0x7: printf("SUBN V%01x, V%01x\n", opcode[0]&0xf, opcode[1]>>4); break;
                case 0xe: printf("SHL V%01x, V%01x\n", opcode[0]&0xf, opcode[1]>>4); break;
                default: printf("UNKNOWN INSTRUCTION\n"); break;
            }
            break;
        case 0x9: printf("SNE V%01x, V%01x\n", opcode[0]&0xf, opcode[1]>>4); break;
        case 0xA: printf("LD I, $%01x%02x\n", opcode[0]&0xf, opcode[1]); break;
        case 0xB: printf("JP V0, $%01x%02x\n", opcode[0]&0xf, opcode[1]); break;
        case 0xC: printf("RND V%01x, #%02x\n", opcode[0]&0xf, opcode[1]); break;
        case 0xD: printf("DRW V%01x, V%01x, #%01x\n", opcode[0]&0xf, opcode[1]>>4, opcode[1]&0xf); break;
        case 0xE: 
            switch (opcode[1]) {
                case 0x9e: printf("SKP V%01x\n", opcode[0]&0xf); break;
                case 0xa1: printf("SKNP V%01x\n", opcode[0]&0xf); break;
                default: printf("UNKNOWN INSTRUCTION\n"); break;
            }
            break;
        case 0xF: 
            switch (opcode[1]) {
                case 0x07: printf("LD V%01x, DT\n", opcode[0]&0xf); break;
                case 0x0a: printf("LD V%01x, K\n", opcode[0]&0xf); break;
                case 0x15: printf("LD DT, V%01x\n", opcode[0]&0xf); break;
                case 0x18: printf("LD ST, V%01x\n", opcode[0]&0xf); break;
                case 0x1e: printf("ADD I, V%01x\n", opcode[0]&0xf); break;
                case 0x29: printf("LD F, V%01x\n", opcode[0]&0xf); break;
                case 0x33: printf("LD B, V%01x\n", opcode[0]&0xf); break;
                case 0x55: printf("LD [I], V%01x\n", opcode[0]&0xf); break;
                case 0x65: printf("LD V%01x, [I]\n", opcode[0]&0xf); break;
                default: printf("UNKNOWN INSTRUCTION\n"); break;
            }
            break;
        default: printf("UNKNOWN INSTRUCTION\n"); break;
    }

    return pc+2;
}
