#ifndef MEMORY
#define MEMORY

#include "types.h"
#include <stdbool.h>

typedef struct Memory{
	u8 RAM[4096];
	u8 screen[32][64];
	u16 PC;
	u16 I;
	u8 DT;
	u8 ST;
	u8 V[16];
} Memory;

void Memory_initialize(Memory *memory);

void rom_read(int argc, char *argv[], Memory *memory, bool *state);

#endif
