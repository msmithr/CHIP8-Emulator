all:
	gcc -std=gnu99 -Wall disassembler.c -odis
	gcc -c -std=gnu99 -Wall chip8.c -ochip8.o 
	gcc -c -std=gnu99 -Wall instructions.c -oinstructions.o
	gcc -c -std=gnu99 -Wall keyboard.c -okeyboard.o
	gcc -c -std=gnu99 -Wall display.c -odisplay.o
	gcc -oemu -std=gnu99 -Wall instructions.o chip8.o keyboard.o display.o `sdl2-config --cflags --libs` -lSDL2 
	rm *.o

debug:
	gcc -DDEBUG -std=gnu99 -Wall disassembler.c -odis
	gcc -DDEBUG -c -std=gnu99 -Wall chip8.c -ochip8.o
	gcc -DDEBUG -c -std=gnu99 -Wall instructions.c -oinstructions.o
	gcc -DDEBUG -c -std=gnu99 -Wall keyboard.c -okeyboard.o
	gcc -DDEBUG -c -std=gnu99 -Wall display.c -odisplay.o
	gcc -DDEBUG -oemu -std=gnu99 -Wall instructions.o chip8.o keyboard.o display.o `sdl2-config --cflags --libs` -lSDL2 
	rm *.o

step:
	gcc -DDEBUG -std=gnu99 -Wall disassembler.c -odis
	gcc -DDEBUG -DSTEP -c -std=gnu99 -Wall chip8.c -ochip8.o
	gcc -DDEBUG -c -std=gnu99 -Wall instructions.c -oinstructions.o
	gcc -DDEBUG -c -std=gnu99 -Wall keyboard.c -okeyboard.o
	gcc -DDEBUG -c -std=gnu99 -Wall display.c -odisplay.o
	gcc -DDEBUG -oemu -std=gnu99 -Wall instructions.o chip8.o keyboard.o display.o `sdl2-config --cflags --libs` -lSDL2 
	rm *.o

clean:
	rm emu
	rm dis
