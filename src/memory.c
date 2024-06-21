#include "../include/memory.h"
#include "../include/constants.h"
#include <stdio.h>

void Memory_initialize(Memory *memory){
	for (u16 i = 0; i < 4096; i++)
		memory->RAM[i] = 0;

	for (u8 y = 0; y < 32; y++) {
		for (u8 x = 0; x < 64; x++)
			memory->screen[y][x] = 0;
	}

	memory->PC = BASE_ADDRESS;
	memory->I = 0;
	memory->DT = 0;
	memory->ST = 0;
	
	for (u8 i = 0; i < 16; i++)
		memory->V[i] = 0;
	
	return;
}

void rom_read(int argc, char *argv[], Memory *memory, bool *state){
	if (argc < 2){
		fprintf(stderr, "Rom file is not provided.\n");	
		*state = false;
		return;
	}

	FILE *file = fopen(argv[1], "rb");
	if (!file){
		fprintf(stderr, "Could not read rom.\n");
		*state = false;
		return;
	}
	
	fseek(file, 0, SEEK_END);
	int size = ftell(file);
	fseek(file, 0, SEEK_SET);
	
	fread(memory->RAM + BASE_ADDRESS, 8, size, file);
	return;
}	
