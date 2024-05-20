#include <iostream>
#include <fstream>
#include <string>


std::string read_opcode(uint16_t opcode)
{
	std::string ret;
	for (int i = 3; i >= 0; i--)
	{
		ret+= "0123456789ABCDEF"[(opcode >> (i*4) & 0xF)];
	}
	return ret;
}


void decode_opcode(uint16_t f_opcode)
{
	switch(f_opcode & 0xF000)
	{
		case 0x0000: 
			{
				if (f_opcode == 0x0000)
					std::cout << read_opcode(f_opcode) << " : NOP" << std::endl;
				       	break;
				// clear display
				if(f_opcode == 0x00E0)
				{
					std::cout << read_opcode(f_opcode) << " : clearDisply()" << std::endl;
					break;
				}
				// return from subroutine
				else if(f_opcode == 0x00EE)
				{
					std::cout << read_opcode(f_opcode) << " : returnFromSubroutine()" << std::endl;
					break;
				}
				// for old machine jump
				else 
				{
					std::cout << read_opcode(f_opcode) << " : OLD JUMP" << std::endl;
					break;
				}

			}
		break;
		case 0x1000:
			std::cout << read_opcode(f_opcode) << " : JUMPto " << read_opcode(f_opcode & 0x0FFF) << std::endl;
		break;
		case 0x2000:
			std::cout << read_opcode(f_opcode) << " : callSubroutine " << read_opcode(f_opcode & 0x0FFF)  << std::endl;
		break;
		case 0x3000:
			std::cout << read_opcode(f_opcode) << " : cond_x_eq " << read_opcode(f_opcode & 0x0FFF) << std::endl;
		break;
		case 0x4000:
			std::cout << read_opcode(f_opcode) << " : cond_x_not_eq " << read_opcode(f_opcode & 0x0FFF) << std::endl;
		break;
		case 0x5000:
			std::cout << read_opcode(f_opcode) << " : cond_x_y_eq " << read_opcode(f_opcode & 0x0FF0) << std::endl;
		break;
		case 0x6000:
			std::cout << read_opcode(f_opcode) << " : set_X_NN " << read_opcode(f_opcode & 0x0FFF) << std::endl;
		break;
		case 0x7000:
			std::cout << read_opcode(f_opcode) << " : add_X_NN " << read_opcode(f_opcode & 0x0FFF) << std::endl;
		break;
		case 0x8000:
		{
			switch(f_opcode & 0x000F)
			{
				case 0x0000:
					std::cout << read_opcode(f_opcode) << " : set_XY " << read_opcode(f_opcode & 0x0FF0) << std::endl;
				break;
				case 0x0001:
					std::cout << read_opcode(f_opcode) << " : or_XY " << read_opcode(f_opcode & 0x0FF0) << std::endl;
				break;
				case 0x0002:
					std::cout << read_opcode(f_opcode) << " : and_XY " << read_opcode(f_opcode & 0x0FF0) << std::endl;
				break;
				case 0x0003:
					std::cout << read_opcode(f_opcode) << " : xor_XY " << read_opcode(f_opcode & 0x0FF0) << std::endl;
				break;
				case 0x0004:
					std::cout << read_opcode(f_opcode) << " : add_XY " << read_opcode(f_opcode & 0x0FF0) << std::endl;
				break;
				case 0x0005:
					std::cout << read_opcode(f_opcode) << " : sub_XY " << read_opcode(f_opcode & 0x0FF0) << std::endl;
				break;
				case 0x0006:
					std::cout << read_opcode(f_opcode) << " : shift_right_XY " << read_opcode(f_opcode & 0x0FF0) << std::endl;
				break;
				case 0x0007:
					std::cout << read_opcode(f_opcode) << " : nsub_XY " << read_opcode(f_opcode & 0x0FF0) << std::endl;
				break;
				case 0x000E:
					std::cout << read_opcode(f_opcode) << " : shift_left_XY " << read_opcode(f_opcode & 0x0FF0) << std::endl;
				break;
			};
		};
		break;
		case 0x9000:
			std::cout << read_opcode(f_opcode) << " : cond_x_y_not_eq " << read_opcode(f_opcode & 0x0FF0) << std::endl;
		break;
		case 0xA000:
			std::cout << read_opcode(f_opcode) << " : mem_set_I " << read_opcode(f_opcode & 0x0FFF) << std::endl;
		break;
		case 0xB000:
			std::cout << read_opcode(f_opcode) << " : jump_to_NNNplusV " << read_opcode(f_opcode & 0x0FFF) << std::endl;
		break;
		case 0xC000:
			std::cout << read_opcode(f_opcode) << " : random_bit " << read_opcode(f_opcode & 0x0FFF) << std::endl;
		break;
		case 0xD000:
			std::cout << read_opcode(f_opcode) << " : draw " << read_opcode(f_opcode & 0x0FFF) << std::endl;
		break;
		case 0xE000:
			{
				switch(f_opcode & 0xFF)
				{
					case 0x9E:
						std::cout << read_opcode(f_opcode) << " : key_press " << read_opcode((f_opcode & 0x0F00) >> 8) << std::endl;
					break;
					case 0xA1:
						std::cout << read_opcode(f_opcode) << " : key_not_press " << read_opcode((f_opcode & 0x0F00) >> 8) << std::endl;
					break;
				}
			};
		break;
		case 0xF000:
		{
			switch (f_opcode & 0xFF)
			{
				case 0x07:
					std::cout << read_opcode(f_opcode) << " : get_delay_timer " << read_opcode((f_opcode & 0x0F00) >> 8) << std::endl;
				break;
				case 0x0A:
	// we need loop but we can't stop emulator so... PC-=2; and we stay in "wait" loop until some key is finaly pressed.
					std::cout << read_opcode(f_opcode) << " : get_key " << read_opcode((f_opcode & 0x0F00) >> 8) << std::endl;
				break;
				case 0x15:
					std::cout << read_opcode(f_opcode) << " : set_delay_timer " << read_opcode((f_opcode & 0x0F00) >> 8) << std::endl;
				break;
				case 0x18:
					std::cout << read_opcode(f_opcode) << " : set_sound_timer " << read_opcode((f_opcode & 0x0F00) >> 8) << std::endl;
				break;
				case 0x1E:
					std::cout << read_opcode(f_opcode) << " : add_VXtoI " << read_opcode((f_opcode & 0x0F00) >> 8) << std::endl;
				break;
				case 0x29:
					// font sprite at the begining of memory, type * 8 (exp. 2 * 8, A * 8);
					std::cout << read_opcode(f_opcode) << " : set_ItoFontSprite " << read_opcode((f_opcode & 0x0F00) >> 8) << std::endl;
				break;
				case 0x33:
					std::cout << read_opcode(f_opcode) << " : set_BCDofVX " << read_opcode((f_opcode & 0x0F00) >> 8) << std::endl;
				break;
				case 0x55:
				// V0 to VX(included VX)
					std::cout << read_opcode(f_opcode) << " : reg_dump " << read_opcode((f_opcode & 0x0F00) >> 8) << std::endl;
				break;
				case 0x65:
				// V0 to VX(included VX)
					std::cout << read_opcode(f_opcode) << " : reg_load " << read_opcode((f_opcode & 0x0F00) >> 8) << std::endl;
				break;
			};
		};
		break;
	};
};




int main(int arg_count, char** arg_vec)
{
	if (arg_count < 2)
	{
		printf("File not selected!\n ./decodechip8 'filename'\n");
		return -1;
	}

	
	uint8_t* memory;
	uint16_t m_size;

	std::ifstream file(arg_vec[1], std::ios::binary | std::ios::ate);

	if(file.is_open())
	{
		std::streampos size = file.tellg();
		char* buffer = new char[size];
		memory = new uint8_t[size];
		m_size = size;

		file.seekg(0,std::ios::beg);
		file.read(buffer, size);
		file.close();

		for (uint16_t i=0;i<size;i++)
		{
			memory[i] = buffer[i];
		}

		delete[] buffer;
	}

	file.close();

	for (uint16_t i=0; i<m_size; i+=2)
	{
		uint16_t opcode = (memory[i] << 8) | memory[i+1];
		decode_opcode(opcode);
	}


	delete[] memory;

	return 0;
}
