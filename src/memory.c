#include "../include/memory.h"

void Memory_initialize(Memory *memory){
	for (u16 i = 0; i < 4096; i++)
		memory->RAM[i] = 0;

	for (u8 y = 0; y < 32; y++) {
		for (u8 x = 0; x < 64; x++)
			memory->screen[y][x] = 0;
	}

	memory->PC = 0x200;
	memory->I = 0;
	memory->DT = 0;
	memory->ST = 0;
	
	for (u8 i = 0; i < 16; i++)
		memory->V[i] = 0;
	
	return;
}
