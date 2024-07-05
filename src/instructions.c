#include "../include/instructions.h"

u16 instruction_fetch(Memory *memory)
{
	u16 instruction;
	memcpy(&instruction, memory->RAM + memory->PC, 2);
	instruction = (instruction << 8 | instruction >> 8);
	return instruction;
}

void instruction_decode(u16 instruction, Memory *memory, SDL_Renderer *renderer, SDL_AudioDeviceID audio_device, bool *state)
{
	u8 second_byte = instruction;

	u8 first_nibble = instruction >> 12;
	u8 second_nibble = (instruction >> 8) & 0xf;
	u8 third_nibble = second_byte >> 4;
	u8 fourth_nibble = instruction & 0xf;

	if(!instruction)
		return;
	switch (first_nibble)
	{
	case 0x0:
		switch (fourth_nibble)
		{
		case 0x0:
			instruction_clean(memory, renderer);
			break;
		case 0xB:
			instruction_scroll_right(memory);
			break;
		case 0xC:
			instruction_scroll_left(memory);
			break;
		case 0xD:
			instruction_exit(state);
			break;
		case 0xE:
			if (third_nibble != 0xF)
				instruction_return(memory);
			else
				instruction_low_res(memory);
			break;
		case 0xF:
		 	instruction_high_res(memory);
			break;
		}
		if (third_nibble == 0xC)
			instruction_scroll_down(memory, fourth_nibble);
		break;
	case 0x1:
		instruction_jump(memory, instruction);
		break;
	case 0x2:
		instruction_call(memory, instruction);
		break;
	case 0x3:
		instruction_skip_equal_const(memory, second_nibble, second_byte);
		break;
	case 0x4:
		instruction_skip_not_equal_const(memory, second_nibble, second_byte);
		break;
	case 0x5:
		instruction_skip_equal_reg(memory, second_nibble, third_nibble);
		break;
	case 0x6:
		instruction_set_reg_const(memory, second_nibble, second_byte);
		break;
	case 0x7:
		instruction_add_reg_const(memory, second_nibble, second_byte);
		break;
	case 0x8:
		switch (fourth_nibble)
		{
		case 0x0:
			instruction_set_reg_reg(memory, second_nibble, third_nibble);
			break;
		case 0x1:
			instruction_or_reg_reg(memory, second_nibble, third_nibble);
			break;
		case 0x2:
			instruction_and_reg_reg(memory, second_nibble, third_nibble);
			break;
		case 0x3:
			instruction_xor_reg_reg(memory, second_nibble, third_nibble);
			break;
		case 0x4:
			instruction_add_reg_reg(memory, second_nibble, third_nibble);
			break;
		case 0x5:
			instruction_sub_reg_reg(memory, second_nibble, third_nibble);
			break;
		case 0x6:
			instruction_shift_right(memory, second_nibble);
			break;
		case 0x7:
			instruction_subn_reg_reg(memory, second_nibble, third_nibble);
			break;
		case 0xE:
			instruction_shift_left(memory, second_nibble);
			break;
		}
		break;
	case 0x9:
		instruction_skip_not_equal_reg(memory, second_nibble, third_nibble);
		break;
	case 0xA:
		instruction_set_I_const(memory, instruction);
		break;
	case 0xB:
		instruction_jump_offset(memory, instruction);
		break;
	case 0xC:
		instruction_random(memory, second_nibble, second_byte);
		break;
	case 0xD:
		instruction_draw_sprite(memory, renderer, second_nibble, third_nibble, fourth_nibble);
		break;
	case 0xE:
	 	if (fourth_nibble == 0xE)
			instruction_skip_key_pressed(memory, second_nibble);
		else
			instruction_skip_key_not_pressed(memory, second_nibble);
		break;
	case 0xF:
		switch (fourth_nibble)
		{
		case 0x0:
			instruction_set_I_large_font(memory, second_nibble);
			break;
		case 0x3:
			instruction_store_BCD(memory, second_nibble);
			break;
		case 0x5:
			switch (third_nibble)
			{
			case 0x1:
				instruction_set_DT_reg(memory, second_nibble);
				break;
			case 0x5:
				instruction_store_regs(memory, second_nibble);
				break;
			case 0x6:
				instruction_read_regs(memory, second_nibble);
				break;
			case 0x7:
				instruction_save_regs(memory, second_nibble);
				break;
			case 0x8:
				instruction_load_regs(memory, second_nibble);
				break;
			}
			break;
		case 0x7:
			instruction_set_reg_DT(memory, second_nibble);
			break;
		case 0x8:
			instruction_set_ST_reg(memory, audio_device, second_nibble);
			break;
		case 0x9:
			instruction_set_I_font(memory, second_nibble);
			break;
		case 0xA:
			instruction_wait_for_key(memory, second_nibble);
			break;
		case 0xE:
			instruction_add_I_reg(memory, second_nibble);
			break;
		}
		break;
	}
}

void instruction_execute(Memory *memory, SDL_Renderer *renderer, SDL_AudioDeviceID audio_device, bool *state)
{
	static clock_t last_instruction_executed = 0;
	clock_t current_tick = clock();
	clock_t delta_clock = current_tick - last_instruction_executed;
	double delta_time = (double)delta_clock / CLOCKS_PER_SEC;
	if (delta_time >= 1.0 / CPU_FREQUENCY)
	{
		u16 instruction = instruction_fetch(memory);
		instruction_decode(instruction, memory, renderer, audio_device, state);
		last_instruction_executed = clock();
	}
}

void instruction_clean(Memory *memory, SDL_Renderer *renderer)
{
	memset(memory->screen, 0, 128 * 64);
	memory->PC += 2;
}

void instruction_return(Memory *memory)
{
	memory->PC = stack_pop(&memory->stack) + 2;
}

void instruction_high_res(Memory *memory)
{
	memory->high_res = 1;
	memory->PC += 2;
}

void instruction_low_res(Memory *memory)
{
	memory->high_res = 0;
	memory->PC += 2;
}

void instruction_scroll_down(Memory *memory, u8 count)
{
	u16 size = count << !memory->high_res * 128;
	memmove(&memory->screen[0][0] + size, memory->screen, sizeof(memory->screen) - size);
	memset(memory->screen, 0, size);
	memory->PC += 2;
}

void instruction_scroll_right(Memory *memory)
{
	for (int y = 0; y < 64; y++)
	{
		memmove(&memory->screen[y][3], &memory->screen[y][0], 124);
		memset(&memory->screen[y][0], 0, 4);
	}
	memory->PC += 2;
}


void instruction_scroll_left(Memory *memory)
{
	for (int y = 0; y < 64; y++)
	{
		memmove(&memory->screen[y][0], &memory->screen[y][3], 124);
		memset(&memory->screen[y][124], 0, 4);
	}
	memory->PC += 2;
}

void instruction_exit(bool *state)
{
	*state = false;
}

void instruction_jump(Memory *memory, u16 raw_address)
{
	u16 address = raw_address & 0xfff;
	memory->PC = address;
}

void instruction_call(Memory *memory, u16 raw_address)
{
	u16 address = raw_address & 0xfff;
	stack_push(&memory->stack, memory->PC);
	memory->PC = address;
}

void instruction_skip_equal_const(Memory *memory, u8 index, u8 value)
{
	memory->PC = memory->PC + 2 + 2 * (memory->V[index] == value);
}

void instruction_skip_not_equal_const(Memory *memory, u8 index, u8 value)
{
	memory->PC = memory->PC + 2 + 2 * (memory->V[index] != value);
}

void instruction_skip_equal_reg(Memory *memory, u8 index_x, u8 index_y)
{
	memory->PC = memory->PC + 2 + 2 * (memory->V[index_x] == memory->V[index_y]);
}

void instruction_set_reg_const(Memory *memory, u8 index, u8 value)
{
	memory->V[index] = value;
	memory->PC += 2;
}

void instruction_add_reg_const(Memory *memory, u8 index, u8 value)
{
	memory->V[index] += value;
	memory->PC += 2;
}

void instruction_set_reg_reg(Memory *memory, u8 index_x, u8 index_y)
{
	memory->V[index_x] = memory->V[index_y];
	memory->PC += 2;
}

void instruction_or_reg_reg(Memory *memory, u8 index_x, u8 index_y)
{
	memory->V[index_x] |= memory->V[index_y];
	memory->PC += 2;
}

void instruction_and_reg_reg(Memory *memory, u8 index_x, u8 index_y)
{
	memory->V[index_x] &= memory->V[index_y];
	memory->PC += 2;
}

void instruction_xor_reg_reg(Memory *memory, u8 index_x, u8 index_y)
{
	memory->V[index_x] ^= memory->V[index_y];
	memory->PC += 2;
}

void instruction_add_reg_reg(Memory *memory, u8 index_x, u8 index_y)
{
	memory->V[index_x] += memory->V[index_y];
	memory->V[15] = memory->V[index_x] + memory->V[index_y] > 255;
	memory->PC += 2;
}

void instruction_sub_reg_reg(Memory *memory, u8 index_x, u8 index_y)
{
	memory->V[index_x] -= memory->V[index_y];
	memory->V[15] = memory->V[index_x] < memory->V[index_y];
	memory->PC += 2;
}

void instruction_shift_right(Memory *memory, u8 index)
{
	memory->V[15] = memory->V[index] & 1;
	memory->V[index] >>= 1;
	memory->PC += 2;
}

void instruction_subn_reg_reg(Memory *memory, u8 index_x, u8 index_y)
{
	memory->V[index_x] = memory->V[index_y] - memory->V[index_x];
	memory->V[15] = memory->V[index_y] > memory->V[index_x];
	memory->PC += 2;
}

void instruction_shift_left(Memory *memory, u8 index)
{
	memory->V[15] = (bool)(memory->V[index] & 128);
	memory->V[index] <<= 1;
	memory->PC += 2;
}

void instruction_skip_not_equal_reg(Memory *memory, u8 index_x, u8 index_y)
{
	memory->PC = memory->PC + 2 + 2 * (memory->V[index_x] != memory->V[index_y]);
}

void instruction_set_I_const(Memory *memory, u16 raw_value)
{
	u16 value = raw_value & 0xfff;
	memory->I = value;
	memory->PC += 2;
}

void instruction_jump_offset(Memory *memory, u16 raw_address)
{
	u16 address = (raw_address & 0xfff) + memory->V[0];
}

void instruction_random(Memory *memory, u8 index, u8 value)
{
	memory->V[index] = rand() & value;
	memory->PC += 2;
}

static void draw_low_res_sprite(Memory *memory, SDL_Renderer *renderer, u8 origin_x, u8 origin_y, u8 height)
{
	const u8 width = 8;
	for (u8 row = 0, y; row < height; row++)
	{
		y = (origin_y + row) * 2;
		if (y >= 64)
			continue;

		for (u8 col = 0, x; col < 8; col++)
		{
			x = (origin_x + col) * 2;
			if (x >= 128)
				continue;
			u16 sprite_offset = memory->I + row;
			bool sprite_pixel = memory->RAM[sprite_offset] & (128 >> col);
			memory->V[15] |= memory->screen[y][x] & sprite_pixel;

			memory->screen[y][x] ^= sprite_pixel;
			memory->screen[y][x + 1] ^= sprite_pixel;
			memory->screen[y + 1][x] ^= sprite_pixel;
			memory->screen[y + 1][x + 1] ^= sprite_pixel;
		}
	}
}

static void draw_high_res_sprite(Memory *memory, SDL_Renderer *renderer, u8 origin_x, u8 origin_y, u8 height)
{
	const u8 width = 8;
	for (u8 row = 0, y; row < height; row++)
	{
		y = origin_y + row;
		if (y >= 64)
			continue;

		for (u8 col = 0, x; col < width; col++)
		{
			x = origin_x + col;
			if (x >= 128)
				continue;

			u16 sprite_offset = memory->I + row;
			bool sprite_pixel = memory->RAM[sprite_offset] & (128 >> col);
			memory->V[15] |= memory->screen[y][x] & sprite_pixel;
			memory->screen[y][x] ^= sprite_pixel;
		}
	}
}

static void draw_big_sprite(Memory *memory, SDL_Renderer *renderer, u8 origin_x, u8 origin_y) 
{
	const u8 width = 16, height = 16;
	for (u8 row = 0, y; row < height; row++)
	{
		y = origin_y + row;
		if (y >= 64)
			continue;

		for (u8 col = 0, x; col < width; col++)
		{
			x = origin_x + col;
			if (x >= 128)
				continue;
			u16 sprite_offset = memory->I + row * 2 + col / 8;
			bool sprite_pixel = memory->RAM[sprite_offset] & (128 >> col % 8);
			memory->V[15] |= memory->screen[y][x] & sprite_pixel;
			memory->screen[y][x] ^= sprite_pixel;
		}
	}
}

void instruction_draw_sprite(Memory *memory, SDL_Renderer *renderer, u8 index_x, u8 index_y, u8 height)
{
	u8 origin_x = memory->V[index_x];
	u8 origin_y = memory->V[index_y];
	memory->V[15] = 0;
	if (height)
	{
		if(memory->high_res)
			draw_high_res_sprite(memory, renderer, origin_x, origin_y, height);
		else
			draw_low_res_sprite(memory, renderer, origin_x, origin_y, height);
	}
	else
		draw_big_sprite(memory, renderer, origin_x, origin_y);
	memory->PC += 2;
}

void instruction_skip_key_pressed(Memory *memory, u8 index)
{
	const Uint8 *key_states = SDL_GetKeyboardState(NULL);
	memory->PC = memory->PC + 2 + 2 * (key_states[key_bindings[memory->V[index]]]);
}

void instruction_skip_key_not_pressed(Memory *memory, u8 index)
{
	const Uint8 *key_states = SDL_GetKeyboardState(NULL);
	memory->PC = memory->PC + 2 + 2 * (!key_states[key_bindings[memory->V[index]]]);
}

void instruction_store_BCD(Memory *memory, u8 index)
{
	u8 value = memory->V[index];
	u8 hundreds = (u8)(value / 100);
	u8 tens = (u8)(value / 10) - hundreds * 10;
	u8 ones = value % 10;

	memory->RAM[memory->I] = hundreds;
	memory->RAM[memory->I + 1] = tens;
	memory->RAM[memory->I + 2] = ones;
	memory->PC += 2;
}

void instruction_set_I_large_font(Memory *memory, u8 index)
{
	memory->I = FONT_OFFSET + 80 + memory->V[index] * 10;
	memory->PC += 2;
}

void instruction_set_DT_reg(Memory *memory, u8 index)
{
	memory->DT = memory->V[index];
	memory->PC += 2;
}

void instruction_store_regs(Memory *memory, u8 amount)
{
	memcpy(memory->RAM + memory->I, memory->V, amount + 1);
	memory->PC += 2;
}

void instruction_read_regs(Memory *memory, u8 amount)
{
	memcpy(memory->V, memory->RAM + memory->I, amount + 1);
	memory->PC += 2;
}

void instruction_save_regs(Memory *memory, u8 index)
{
	memcpy(memory->flags, memory->V, index);
	memory->PC += 2;
}

void instruction_load_regs(Memory *memory, u8 index)
{
	memcpy(memory->V, memory->flags, index);
	memory->PC += 2;
}

void instruction_set_reg_DT(Memory *memory, u8 index)
{
	memory->V[index] = memory->DT;
	memory->PC += 2;
}

void instruction_set_ST_reg(Memory *memory, SDL_AudioDeviceID audio_device, u8 index)
{
	double duration = (double)memory->V[index] / TIMER_FREQUENCY;
	audio_play(audio_device, duration);
	memory->PC += 2;
}

void instruction_set_I_font(Memory *memory, u8 index)
{
	memory->I = FONT_OFFSET + memory->V[index] * 5;
	memory->PC += 2;
}

void instruction_wait_for_key(Memory *memory, u8 index)
{
	const Uint8 *key_states = SDL_GetKeyboardState(NULL);
	for (int i = 0; i < 16; i++)
	{
		if (key_states[key_bindings[i]])
		{
			memory->V[index] = i;
			memory->PC += 2;
		}
	}
}

void instruction_add_I_reg(Memory *memory, u8 index)
{
	memory->I += memory->V[index];
	memory->PC += 2;
}

void instruction_update_timers(Memory *memory)
{
	static clock_t last_timer_update = 0;
	clock_t current_tick = clock();
	clock_t delta_clock = current_tick - last_timer_update;
	double delta_time = (double)delta_clock / CLOCKS_PER_SEC;
	if (delta_time >= 1.0 / TIMER_FREQUENCY)
	{
		if (memory->DT > 0)
			memory->DT--;
		last_timer_update = clock();
	}
}
