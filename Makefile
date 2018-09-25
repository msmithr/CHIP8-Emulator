all:
	gcc -std=gnu99 -Wall disassembler.c -odis
	gcc -c -std=gnu99 -Wall chip8.c -ochip8.o 
	gcc -c -std=gnu99 -Wall instructions.c -oinstructions.o
	gcc -oemu -std=gnu99 -Wall instructions.o chip8.o `sdl2-config --cflags --libs` -lSDL2 

debug:
	gcc -DDEBUG -std=gnu99 -Wall disassembler.c -odis
	gcc -DDEBUG -c -std=gnu99 -Wall chip8.c -ochip8.o
	gcc -DDEBUG -c -std=gnu99 -Wall instructions.c -oinstructions.o
	gcc -DDEBUG -oemu -std=gnu99 -Wall instructions.o chip8.o `sdl2-config --cflags --libs` -lSDL2 

clean:
	rm *.o
	rm emu
	rm dis
