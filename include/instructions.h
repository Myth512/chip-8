#ifndef INSTRUCTIONS
#define INSTRUCTIONS

#include <string.h>
#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>

#include "types.h"
#include "constants.h"
#include "memory.h"
#include "window.h"
#include "audio.h"
#include "stack.h"

u16 instruction_fetch(Memory *memory);

void instruction_decode(u16 instruction, Memory *memory, SDL_Renderer *renderer, SDL_AudioDeviceID audio_device, bool *state);

void instruction_execute(Memory *memory, SDL_Renderer *renderer, SDL_AudioDeviceID audio_device, bool *state);

void instruction_clean(Memory *memory, SDL_Renderer *renderer);

void instruction_return(Memory *memory);

void instruction_high_res(Memory *memory);

void instruction_low_res(Memory *memory);

void instruction_scroll_down(Memory *memory, u8 count);

void instruction_scroll_right(Memory *memory);

void instruction_scroll_left(Memory *memory);

void instruction_exit(bool *state);

void instruction_jump(Memory *memory, u16 raw_address);

void instruction_call(Memory *memory, u16 raw_address);

void instruction_skip_equal_const(Memory *memory, u8 index, u8 value);

void instruction_skip_not_equal_const(Memory *memory, u8 index, u8 value);

void instruction_skip_equal_reg(Memory *memory, u8 index_x, u8 index_y);

void instruction_set_reg_const(Memory *memory, u8 index, u8 value);

void instruction_add_reg_const(Memory *memory, u8 index, u8 value);

void instruction_set_reg_reg(Memory *memory, u8 index_x, u8 index_y);

void instruction_or_reg_reg(Memory *memory, u8 index_x, u8 index_y);

void instruction_and_reg_reg(Memory *memory, u8 index_x, u8 index_y);

void instruction_xor_reg_reg(Memory *memory, u8 index_x, u8 index_y);

void instruction_add_reg_reg(Memory *memory, u8 index_x, u8 index_y);

void instruction_sub_reg_reg(Memory *memory, u8 index_x, u8 index_y);

void instruction_shift_right(Memory *memoory, u8 index);

void instruction_subn_reg_reg(Memory *memory, u8 index_x, u8 index_y);

void instruction_shift_left(Memory *memory, u8 index);

void instruction_skip_not_equal_reg(Memory *memory, u8 index_x, u8 index_y);

void instruction_set_I_const(Memory *memory, u16 raw_value);

void instruction_jump_offset(Memory *memory, u16 raw_address);

void instruction_random(Memory *memory, u8 index, u8 value);

void instruction_draw_sprite(Memory *memory, SDL_Renderer *rendere, u8 index_x, u8 index_y, u8 height);

void instruction_skip_key_pressed(Memory *memory, u8 index);

void instruction_skip_key_not_pressed(Memory *memory, u8 index);

void instruction_store_BCD(Memory *memory, u8 index);

void instruction_set_I_large_font(Memory *memory, u8 index);

void instruction_set_DT_reg(Memory *memory, u8 index);

void instruction_store_regs(Memory *memory, u8 index);

void instruction_read_regs(Memory *memory, u8 index);

void instruction_save_regs(Memory *memory, u8 index);

void instruction_load_regs(Memory *memory, u8 index);

void instruction_set_reg_DT(Memory *memory, u8 index);

void instruction_set_ST_reg(Memory *memory, SDL_AudioDeviceID audio_device, u8 index);

void instruction_set_I_font(Memory *memory, u8 index);

void instruction_wait_for_key(Memory *memory, u8 index);

void instruction_add_I_reg(Memory *memory, u8 index);

void instruction_update_timers(Memory *memory); 
#endif
