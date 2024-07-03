#ifndef MEMORY
#define MEMORY

#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "types.h"
#include "constants.h"
#include "stack.h"

extern const u8 font[80];

typedef struct Memory
{
	u8 RAM[4096];
	u8 screen[64][128];
	bool high_res;
	u16 PC;
	u16 I;
	u8 DT;
	u8 ST;
	u8 V[16];
    Stack stack; 
} Memory;

void memory_initialize(Memory *memory);

bool memory_rom_read(int argc, char *argv[], Memory *memory);

#endif
