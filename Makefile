all:
	gcc -c -std=gnu99 -Wall src/chip8.c -ochip8.o 
	gcc -c -std=gnu99 -Wall src/cpu.c -ocpu.o
	gcc -c -std=gnu99 -Wall src/keyboard.c -okeyboard.o
	gcc -c -std=gnu99 -Wall src/display.c -odisplay.o
	gcc -oemu -std=gnu99 -Wall cpu.o chip8.o keyboard.o display.o `sdl2-config --cflags --libs` -lSDL2 
	rm *.o

debug:
	gcc -DDEBUG -c -std=gnu99 -Wall src/chip8.c -ochip8.o
	gcc -DDEBUG -c -std=gnu99 -Wall src/cpu.c -ocpu.o
	gcc -DDEBUG -c -std=gnu99 -Wall src/keyboard.c -okeyboard.o
	gcc -DDEBUG -c -std=gnu99 -Wall src/display.c -odisplay.o
	gcc -DDEBUG -oemu -std=gnu99 -Wall cpu.o chip8.o keyboard.o display.o `sdl2-config --cflags --libs` -lSDL2 
	rm *.o

clean:
	rm emu
