#include "../include/instructions.h"

u16 instruction_fetch(Memory *memory) {
	u16 instruction = 0xffff;
	memcpy(&instruction, memory->RAM + memory->PC, INSTRUCTION_SIZE);
	return instruction;
}

void instruction_decode(u16 instruction, Memory *memory, SDL_Renderer *renderer, bool *state){
	u8 first_nibble = (instruction >> 4) & 0b1111;
	u8 second_nibble = instruction & 0b1111;
	u8 third_nibble = instruction >> 12;
	u8 fourth_nibble = (instruction >> 8) & 0b1111;

    if (!instruction) return;

	switch(first_nibble){
		case 0x0:
			switch(fourth_nibble){
				case 0x0:
					instruction_clean(memory, renderer);
					break;
				case 0xE:
					instruction_return();
			}
			break;
		case 0x1:
			instruction_jump(memory, instruction);
			break;
		case 0x2:
			instruction_call();
			break;
		case 0x3:
			instruction_skip_equal_const();
			break;
		case 0x4:
			instruction_skip_not_equal_const();
			break;
		case 0x5:
			instruction_skip_equal_reg();
			break;
		case 0x6: {
            u8 value = third_nibble << 4 | fourth_nibble;
			instruction_set_reg_const(memory, second_nibble, value);
			break; }
		case 0x7: {
            u8 value = third_nibble << 4 | fourth_nibble;
			instruction_add_reg_const(memory, second_nibble, value);
			break; } 
		case 0x8:
			switch(fourth_nibble){
				case 0x0:
					instruction_set_reg_reg();
					break;
				case 0x1:
					instruction_or_reg_reg();
					break;
				case 0x2:
					instruction_and_reg_reg();
					break;
				case 0x3:
					instruction_xor_reg_reg();
					break;
				case 0x4:
					instruction_add_reg_reg();
					break;
				case 0x5:
					instruction_sub_reg_reg();
					break;
				case 0x6:
					instruction_shift_right();
					break;
				case 0x7:
					instruction_subn_reg_reg();
					break;
				case 0xE:
					instruction_shift_left();
					break;
			}
			break;
		case 0x9:
			instruction_skip_not_equal_reg();
			break;
		case 0xA:
			instruction_set_I_const(memory, instruction);
			break;
		case 0xB:
			instruction_jump_V0();
			break;
		 case 0xC:
			instruction_random();
			break;
		case 0xD:
			instruction_draw_sprite(memory, renderer, second_nibble, third_nibble, fourth_nibble);
			break;
		case 0xE:
			switch(fourth_nibble){
				case 0xE:
					instruction_skip_key_pressed();
					break;
				case 0x1:
					instruction_skip_key_not_pressed();
					break;
			}
			break;
		case 0xF:
			switch(fourth_nibble){
				case 0x3:
					instruction_store_BCD();
					break;
				case 0x5:
					switch(third_nibble){
						case 0x1:
							instruction_set_DT_reg();
							break;
						case 0x5:
							instruction_store_regs();
							break;
						case 0x6:
							instruction_read_regs();
							break;
					}
					break;
				case 0x7:
					instruction_set_reg_DT();
					break;
				case 0x8:
					instruction_set_ST_reg();
					break;
				case 0x9:
					instruction_set_I_reg();
					break;
				case 0xA:
					instruction_wait_for_key();
					break;
				case 0xE:
					instruction_add_I_reg();
					break;
			}
			break;
	}		
	return;
}

void instruction_clean(Memory *memory, SDL_Renderer *renderer) {
    for (u8 y = 0; y < 32; y++) {
        for (u8 x = 0; x < 64; x++)
            memory->screen[y][x] = 0;
    }
    window_clear(renderer);

    memory->PC += 2;
	return;
}

void instruction_return() {
	return;
}

void instruction_jump(Memory *memory, u16 raw_address) { 
    u16 address = (raw_address << 8 | raw_address >> 8) & 0xfff;
    memory->PC = address;
    return;
}

void instruction_call() {
	return;
}

void instruction_skip_equal_const() {
	return;
}

void instruction_skip_not_equal_const() {
	return;
}

void instruction_skip_equal_reg() {
	return;
}

void instruction_set_reg_const(Memory *memory, u8 index, u8 value) {
    memory->V[index] = value;

    memory->PC += 2;
	return;
}

void instruction_add_reg_const(Memory *memory, u8 index, u8 value) {
    memory->V[index] += value;

    memory->PC += 2;
	return;
}

void instruction_set_reg_reg() {
	return;
}

void instruction_or_reg_reg() {
	return;
}

void instruction_and_reg_reg() {
	return;
}

void instruction_xor_reg_reg() {
	return;
}

void instruction_add_reg_reg() {
	return;
}

void instruction_sub_reg_reg() {
	return;
}

void instruction_shift_right() {
	return;
}

void instruction_subn_reg_reg() {
	return;
}

void instruction_shift_left() {
	return;
}

void instruction_skip_not_equal_reg() {
	return;
}

void instruction_set_I_const(Memory *memory, u16 raw_value) {
    u16 value = (raw_value << 8 | raw_value >> 8) & 0xfff;
    memory->I = value;
    
    memory->PC += 2;
	return;
}

void instruction_jump_V0() {
	return;
}

void instruction_random() {
	return;
}

void instruction_draw_sprite(Memory *memory, SDL_Renderer *renderer, u8 index_x, u8 index_y, u8 height) {
    u8 origin_x = memory->V[index_x];
    u8 origin_y = memory->V[index_y];
    memory->V[15] = 0;
    for (u8 y = 0; y < height; y++){
        for (u8 x = 0; x < 8; x++){
            u8 real_x = (origin_x + x) & 63;
            u8 real_y = (origin_y + y) & 31;
            u8 *screen_pixel = &memory->screen[real_y][real_x];
            u8 sprite_pixel = (bool)(memory->RAM[memory->I + y] & (128 >> x));
            memory->V[15] |= *screen_pixel & sprite_pixel;
            *screen_pixel ^= sprite_pixel;
        }
    }
    window_draw_sprite(memory, renderer, origin_x, origin_y, height);

    memory->PC += 2;
	return;
}

void instruction_skip_key_pressed() {
	return;
}

void instruction_skip_key_not_pressed() {
	return;
}

void instruction_store_BCD() {
	return;
}

void instruction_set_DT_reg() {
	return;
}

void instruction_store_regs() {
	return;
}

void instruction_read_regs() {
	return;
}

void instruction_set_reg_DT() {
	return;
}

void instruction_set_ST_reg() {
	return;
}

void instruction_set_I_reg() {
	return;
}

void instruction_wait_for_key() {
	return;
}

 void instruction_add_I_reg() {
	return;
}
