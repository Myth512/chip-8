#ifndef INSTRUCTIONS
#define INSTRUCTIONS

#include <string.h>
#include <SDL2/SDL.h>
#include <stdio.h>
#include "types.h"
#include "constants.h"
#include "memory.h"
#include "window.h"

u16 instruction_fetch(Memory *memory);

void instruction_decode(u16 instruction, Memory *memory, SDL_Renderer *renderer, bool *state);

void instruction_clean(Memory *memory, SDL_Renderer *renderer);

void instruction_return();

void instruction_jump(Memory *memory, u16 raw_address);

void instruction_call();

void instruction_skip_equal_const();

void instruction_skip_not_equal_const();

void instruction_skip_equal_reg();

void instruction_set_reg_const(Memory *memory, u8 index, u8 value);

void instruction_add_reg_const(Memory *memory, u8 index, u8 value);

void instruction_set_reg_reg();

void instruction_or_reg_reg();

void instruction_and_reg_reg();

void instruction_xor_reg_reg();

void instruction_add_reg_reg();

void instruction_sub_reg_reg();

void instruction_shift_right();

void instruction_subn_reg_reg();

void instruction_shift_left();

void instruction_skip_not_equal_reg();

void instruction_set_I_const(Memory *memory, u16 raw_value);

void instruction_jump_V0();

void instruction_random();

void instruction_draw();

void instruction_skip_key_pressed();

void instruction_skip_key_not_pressed();

void instruction_store_BCD();

void instruction_set_DT_reg();

void instruction_store_regs();

void instruction_read_regs();

void instruction_set_reg_DT();

void instruction_set_ST_reg();

void instruction_set_I_reg();

void instruction_wait_for_key();

void instruction_add_I_reg();
#endif
