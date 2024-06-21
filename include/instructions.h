#ifndef INSTRUCTIONS
#define INSTRUCTIONS

#include "types.h"
#include "constants.h"
#include "memory.h"

void instruction_fetch(Memory *memory, u8 instruction[INSTRUCTION_SIZE]);

void instruction_decode(u8 instruction[INSTRUCTION_SIZE]);

void instruction_execute(u8 instruction[INSTRUCTION_SIZE]);

#endif
