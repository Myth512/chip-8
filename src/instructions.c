#include "../include/instructions.h"

void instruction_fetch(Memory *memory, u16 instruction){
	memcpy(instruction, memory->RAM[memory->PC], INSTRUCTION_SIZE);
	return;
}

void instruction_decode(u16 instruction){
	u8 first_nibble = instruction >> 12;
	u8 second_nibble =(instruction >> 8) & 0b1111;
	u8 third_nibble = (instruction >> 4) & 0b1111;
	u8 last_nibble = instruciton & 0b1111;
	switch(first_nibble){
		case 0x0:
			switch(last_nibble){
				case 0x0:
					instruction_clean();
					break;
				case 0xE:
					instruction_return();
			}
			break;
		case 0x1:
			instruction_jump();
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
		case 0x6:
			instruction_set_reg_const();
			break;
		case 0x7:
			instruction_add_reg_const();
			break;
		case 0x8:
			switch(last_nibble){
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
					instruction_xor_reg_reg():
					break;
				case 0x4:
					instruction_add_reg_reg();
					break;
				case 0x5:
					instruction_sub_reg_reg();
					break;
				case 0x6:
					instruction_right_shift();
					break;
				case 0x7:
					instruction_subn_reg_reg();
					break;
				case 0xE:
					instruction_left_shift();
					break;
			}
			break;
		case 0x9:
			instruction_skip_not_equal_reg();
			break;
		case 0xA:
			instruction_set_I_const();
			break;
		case 0xB:
			instruction_jump_V0();
			break;
		 case 0xC:
			instruction_rand();
			break;
		case 0xD:
			instruction_draw();
			break;
		case 0xE:
			switch(last_nibble){
				case 0xE:
					instruction_skip_key_pressed();
					break;
				case 0x1:
					instruction_skip_key_not_pressed();
					break;
			}
			break;
		case 0xF:
			switch(last_nibble){
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
