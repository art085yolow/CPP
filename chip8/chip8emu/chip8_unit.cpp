#include <iostream>
#include <random>
#include <string>



struct CPU_8
{
	uint16_t program_count=0x200;
	uint16_t fetch_opcode = 0;
	bool draw_flag = false;
	bool b_debug = false;

	// video 0xF00-0xFFF

	// 60hz count down
	uint8_t timer_delay = 0;
	uint8_t timer_sound = 0;

	uint16_t stack_pointer=0xEFE; //from 0xEA0-EFF
	uint16_t reg_I = 0;
	uint8_t reg_V[0x10];
	uint8_t memory[0x1000] = {
	       	0xF0, 0x90, 0x90, 0x90, 0xF0, // font 0
		0x20, 0x60, 0x20, 0x20, 0x70, // font 1
		0xF0, 0x10, 0xF0, 0x80, 0xF0, // font 2
		0xF0, 0x10, 0xF0, 0x10, 0xF0, // font 3
	       	0x90, 0x90, 0xF0, 0x10, 0x10, // font 4
		0xF0, 0x80, 0xF0, 0x10, 0xF0, // font 5
		0xF0, 0x80, 0xF0, 0x90, 0xF0, // font 6
		0xF0, 0x10, 0x20, 0x40, 0x40, // font 7
	       	0xF0, 0x90, 0xF0, 0x90, 0xF0, // font 8
		0xF0, 0x90, 0xF0, 0x10, 0xF0, // font 9
		0xF0, 0x90, 0xF0, 0x90, 0x90, // font A
		0xE0, 0x90, 0xE0, 0x90, 0xE0, // font B
	       	0xF0, 0x80, 0x80, 0x80, 0xF0, // font C
		0xE0, 0x90, 0x90, 0x90, 0xE0, // font D
		0xF0, 0x80, 0xF0, 0x80, 0xF0, // font E
		0xF0, 0x80, 0xF0, 0x80, 0x80, // font F
	};
	bool keys[0x10];

	void makeClear()
	{
	        for (size_t i = 0x50; i < 0x1000; i++)
	        {
            		this->memory[i] = 0;
       		}
		for (size_t i = 0; i < 0x10; i++)
		{
			this->reg_V[i] = 0;
			this->keys[i] = false;
		}
        
	}

	void clearDisplay()
	{
	        for (size_t i = 0x0F00; i < 0x1000; i++)
	        {
            		this->memory[i] = 0;
       		}
	}


	std::string read_opcode(uint16_t _opcode)
	{
		std::string ret;
		for (int i = 3; i >= 0; i--)
		{
			ret+= "0123456789ABCDEF"[(_opcode >> (i*4) & 0xF)];
		}
		return ret;
	}

	void fetch()
	{
		// this->fetch_opcode = this->memory[program_count] << 8;
		// this->fetch_opcode |= this->memory[program_count+1];

		this->fetch_opcode = (this->memory[program_count] << 8) | this->memory[program_count + 1];
		this->program_count+=2;
		this->program_count&=0x0FFF;
	};

	void jumpTo(uint16_t _jump_to_address)
	{
		this->program_count = _jump_to_address & 0x0FFF; 
	};

	void callSubroutine(uint16_t _call_sub_address)
	{
		// calling subroutin is adding current PC value to stack pointer
		// check if stack pointer is in range of 0xEA0 and 0xEFF
		if((this->stack_pointer - 2 >= 0xEA0) && (this->stack_pointer <= 0xEFE))
		{
			this->memory[this->stack_pointer] = ((this->program_count >> 8) & 0x0F);
			this->memory[this->stack_pointer + 1] = (this->program_count & 0xFF);
			this->stack_pointer-=2;
			this->program_count = _call_sub_address & 0x0FFF;
		}
		else
		{
			std::cout << "STACK_POINTER calling ERROR" << std::endl;
			exit(-1000);
		};
	};

	void returnFromSubroutine()
	{
		if((this->stack_pointer >= 0xEA0) && (this->stack_pointer + 2 <= 0xEFE))
		{
			this->stack_pointer += 2;
			this->program_count = ((this->memory[this->stack_pointer]) << 8 ) | this->memory[this->stack_pointer + 1 ];
		}
		else
		{
			std::cout << "STACK_POINTER return ERROR" << std::endl;
			exit(-1000);
		};
	};

	void cond_x_eq(uint16_t _X_NN)
	{
		uint8_t reg_V_X = (_X_NN & 0x0F00) >> 8;
		uint8_t eq_to_NN = _X_NN & 0x00FF;

		if (this->reg_V[reg_V_X] == eq_to_NN)
		{
			this->program_count +=2;
		};
	};

	void cond_x_not_eq(uint16_t _X_NN)
	{
		uint8_t reg_V_X = (_X_NN & 0x0F00) >> 8;
		uint8_t not_eq_to_NN = _X_NN & 0x00FF;

		if (this->reg_V[reg_V_X] != not_eq_to_NN)
		{
			this->program_count +=2;
		};
	};
	
	void cond_x_y_eq(uint16_t _X_Y)
	{
 		uint8_t reg_V_X = (_X_Y & 0x0F00) >> 8;
		uint8_t reg_V_Y = (_X_Y & 0x00F0) >> 4;

		if (this->reg_V[reg_V_X] == this->reg_V[reg_V_Y]) 
		{
			this->program_count +=2;
		};

	};
	
	void cond_x_y_not_eq(uint16_t _X_Y)
	{
 		uint8_t reg_V_X = (_X_Y & 0x0F00) >> 8;
		uint8_t reg_V_Y = (_X_Y & 0x00F0) >> 4;

		if (this->reg_V[reg_V_X] != this->reg_V[reg_V_Y]) 
		{
			this->program_count +=2;
		};

	};

	void set_X_NN(uint16_t _X_NN)
	{
		uint8_t reg_V_X = (_X_NN & 0x0F00) >> 8;
		uint8_t to_NN = _X_NN & 0x00FF;

		this->reg_V[reg_V_X] = to_NN;
	};

	void add_X_NN(uint16_t _X_NN)
	{
		uint8_t reg_V_X = (_X_NN & 0x0F00) >> 8;
		uint8_t to_NN = _X_NN & 0x00FF;
		// it is not changing carry flags!
		this->reg_V[reg_V_X] += to_NN;
	};

	void set_XY(uint16_t _XY)
	{
		uint8_t x = (_XY & 0x0F00) >> 8;
		uint8_t y = (_XY & 0x00F0) >> 4;

		this->reg_V[x] = this->reg_V[y];
	};
	
	void or_XY(uint16_t _XY)
	{
		uint8_t x = (_XY & 0x0F00) >> 8;
		uint8_t y = (_XY & 0x00F0) >> 4;

		this->reg_V[x] |= this->reg_V[y];
	};

	
	void and_XY(uint16_t _XY)
	{
		uint8_t x = (_XY & 0x0F00) >> 8;
		uint8_t y = (_XY & 0x00F0) >> 4;

		this->reg_V[x] &= this->reg_V[y];
	};

	
	void xor_XY(uint16_t _XY)
	{
		uint8_t x = (_XY & 0x0F00) >> 8;
		uint8_t y = (_XY & 0x00F0) >> 4;

		this->reg_V[x] ^= this->reg_V[y];
	};

	
	void add_XY(uint16_t _XY)
	{
		uint8_t x = (_XY & 0x0F00) >> 8;
		uint8_t y = (_XY & 0x00F0) >> 4;
		// set carry flag
		uint16_t sum = this->reg_V[x] + this->reg_V[y];
		this->reg_V[0xF] = sum > 0xFF ?		1 : 0;
		this->reg_V[x] = sum & 0xFF;
	};

	
	void sub_XY(uint16_t _XY)
	{
		uint8_t x = (_XY & 0x0F00) >> 8;
		uint8_t y = (_XY & 0x00F0) >> 4;
		// set carry flag
		this->reg_V[0xF] = this->reg_V[x] > this->reg_V[y] ?		1 : 0;
		int16_t sum = this->reg_V[x] - this->reg_V[y];
		this->reg_V[x] = sum & 0xFF;
	};

	
	void shift_right_XY(uint16_t _XY)
	{
		uint8_t x = (_XY & 0x0F00) >> 8;
//		uint8_t y = (_XY & 0x00F0) >> 4;
		// set carry flag
		this->reg_V[0xF] = this->reg_V[x] & 0x01;
		this->reg_V[x] >>= 1;
	};

	
	void nsub_XY(uint16_t _XY)
	{
		uint8_t x = (_XY & 0x0F00) >> 8;
		uint8_t y = (_XY & 0x00F0) >> 4;
		// set carry flag
		this->reg_V[0xF] = this->reg_V[y] > this->reg_V[x] ?		1 : 0;
		int16_t sum = this->reg_V[y] - this->reg_V[x];
		this->reg_V[x] = sum & 0xFF;
	};

	
	void shift_left_XY(uint16_t _XY)
	{
		uint8_t x = (_XY & 0x0F00) >> 8;
//		uint8_t y = (_XY & 0x00F0) >> 4;
		// set carry flag
		this->reg_V[0xF] = (this->reg_V[x] & 0x80) >> 7;
		this->reg_V[x] <<= 1;
	};

	void mem_set_I(uint16_t _NNN)
	{
		this->reg_I = _NNN & 0x0FFF;
	};

	void jump_to_NNNplusV(uint16_t _NNN)
	{
		this->program_count = (this->reg_V[0] + _NNN) & 0x0FFF;
	};

	void random_bit(uint16_t _X_NN)
	{
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_int_distribution<uint8_t> distr(0,255);
		uint8_t rand_nr = distr(gen);

		uint8_t x = (_X_NN & 0x0F00) >> 8;
		uint8_t nn = _X_NN & 0xFF;
		this->reg_V[x] = rand_nr & nn;
//		std::cout << "rand: " << uint16_t(rand_nr) << " nn: " << uint16_t(nn) << " &: " << uint16_t(this->reg_V[x]) << std::endl;	
	};

	void draw(uint16_t _XYN)
	{
		// screen have (64)w * (32)h pixels
		uint8_t x = this->reg_V[(_XYN >> 8) & 0x000F];
		uint8_t y = this->reg_V[(_XYN >> 4) & 0x000F];
		uint8_t n = _XYN & 0x000F;
		

		this->reg_V[0xF] = 0;
		for (size_t y_line = 0; y_line < n; y_line++)
		{
			uint8_t pixels = this->memory[this->reg_I + y_line];

//		if(this->b_debug) std::cout << "Chip8_unit::draw X: " << uint16_t(x) << " Y: " << uint16_t(y) << " N: " << uint16_t(n) << " REG_I: " << this->reg_I << " pixels at: " << uint16_t(this->reg_I + y_line) << std::endl;
			
			for (size_t x_line = 0; x_line < 8; x_line++)
			{
				if(x+x_line<64)
				{
					if (((pixels >> (7 - x_line)) & 1) != 0)
					{
						// colision? raise flag
						// make correction // 64 pix dev 8 (8pix per char) eq 8
					
						uint8_t byte_index = (((y + y_line) * 64) + (x + x_line)) / 8;
						uint8_t bit_index = (x + x_line) % 8;	
						
						if (((this->memory[0x0F00 + byte_index] >> bit_index) & 0x1) == 1)
						{
							this->reg_V[0xF] = 1;
						};

						this->memory[0x0F00 + byte_index] ^= (0x1 << bit_index);
					};
				};
			};
		};
		this->draw_flag = true;		
	};

	void key_press(uint8_t _VX)
	{
		if (this->keys[this->reg_V[_VX]] == true)
			this->program_count += 2;
	};

	void key_not_press(uint8_t _VX)
	{
		if (this->keys[this->reg_V[_VX]] == false)
			this->program_count += 2;
	};

	void get_delay_timer(uint8_t _VX)
	{
		this->reg_V[_VX] = this->timer_delay;
	};

	void get_key(uint8_t _VX)
	{
		// we need loop but we can't stop emulator so... PC-=2; and we stay in "wait" loop until some key is finaly pressed.
		bool key_pressed = false;
		for (size_t idx = 0; idx < 0x10; idx++)
		{
			if (this->keys[idx] == true)
			{
				this->reg_V[_VX] = idx;
				key_pressed = true;
			};
		};

		if (!key_pressed) 
		{
			this->program_count -=2;
		};

	};

	void set_delay_timer(uint8_t _VX)
	{
		this->timer_delay = this->reg_V[_VX];
	};

	void set_sound_timer(uint8_t _VX)
	{
		this->timer_sound = this->reg_V[_VX];
	};

	void add_VXtoI(uint8_t _VX)
	{
		this->reg_I += this->reg_V[_VX];
		this->reg_I &= 0x0FFF;
	};

	void set_ItoFontSprite(uint8_t _VX)
	{
		// font sprite at the begining of memory, type * 8 (exp. 2 * 8, A * 8);
		this->reg_I = this->reg_V[_VX] * 5;
	};

	void set_BCDofVX(uint8_t _VX)
	{
		this->memory[this->reg_I] = this->reg_V[_VX] / 100;
		this->memory[this->reg_I + 1] = (this->reg_V[_VX] / 10) % 10;
		this->memory[this->reg_I + 2] = (this->reg_V[_VX] % 100) % 10;
	};

	void reg_dump(uint8_t _VX)
	{
		// V0 to VX(included VX)
		for (uint8_t idx = 0; idx < (_VX + 1); idx++)
		{
			this->memory[this->reg_I + idx] = this->reg_V[idx];
		};

	};

	void reg_load(uint8_t _VX)
	{
		// V0 to VX(included VX)
		for (uint8_t idx = 0; idx < (_VX + 1); idx++)
		{
			this->reg_V[idx] = this->memory[this->reg_I + idx];
		};

	};

void decode_opcode(uint16_t f_opcode)
{
	std::cout << read_opcode(this->program_count - 2) << " :: ";
	switch(f_opcode & 0xF000)
	{
		case 0x0000: 
			{
				if (f_opcode == 0x0000)
				{
					std::cout << read_opcode(f_opcode) << " : NOP" << std::endl;
				}
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
			std::cout << read_opcode(f_opcode) << " : cond_x_eq V[" << read_opcode((f_opcode >> 8) & 0x000F) << "] to NN: " << read_opcode(f_opcode & 0x00FF) << std::endl;
		break;
		case 0x4000:
			std::cout << read_opcode(f_opcode) << " : cond_x_not_eq V["  << read_opcode((f_opcode >> 8) & 0x000F) << "] to NN: " << read_opcode(f_opcode & 0x00FF) << std::endl;
		break;
		case 0x5000:
			std::cout << read_opcode(f_opcode) << " : cond_x_y_eq V[" << read_opcode((f_opcode >> 8) & 0x000F) << "] V[" << read_opcode((f_opcode >> 4) & 0x000F) << "]" << std::endl;
		break;
		case 0x6000:
			std::cout << read_opcode(f_opcode) << " : set_X_NN V[" << read_opcode((f_opcode >> 8) & 0x000F) << "] NN: " << read_opcode(f_opcode & 0x00FF) << std::endl;
		break;
		case 0x7000:
			std::cout << read_opcode(f_opcode) << " : add_X_NN V[" << read_opcode((f_opcode >> 8) & 0x000F) << "] NN: " << read_opcode(f_opcode & 0x00FF) << std::endl;
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


	void execute()
	{
		if(this->b_debug) decode_opcode(this->fetch_opcode);
		
		switch(this->fetch_opcode & 0xF000)
		{
			case 0x0000: 
				{
					if (this->fetch_opcode == 0x0000) break;
					// clear display
					if(this->fetch_opcode == 0x00E0)
					{
						this->clearDisplay();
						break;
					}
					// return from subroutine
					else if(this->fetch_opcode == 0x00EE)
					{
						returnFromSubroutine();
						break;
					}
					// for old machine jump
					else 
					{
						this->program_count = this->fetch_opcode & 0x0FFF;
						break;
					}

				}
			break;
			case 0x1000:
				this->jumpTo(this->fetch_opcode & 0x0FFF);
			break;
			case 0x2000:
				this->callSubroutine(this->fetch_opcode & 0x0FFF);
			break;
			case 0x3000:
				this->cond_x_eq(this->fetch_opcode & 0x0FFF);
			break;
			case 0x4000:
				this->cond_x_not_eq(this->fetch_opcode & 0x0FFF);
			break;
			case 0x5000:
				this->cond_x_y_eq(this->fetch_opcode & 0x0FF0);
			break;
			case 0x6000:
				this->set_X_NN(this->fetch_opcode & 0x0FFF);
			break;
			case 0x7000:
				this->add_X_NN(this->fetch_opcode & 0x0FFF);
			break;
			case 0x8000:
			{
				switch(this->fetch_opcode & 0x000F)
				{
					case 0x0000:
						this->set_XY(this->fetch_opcode & 0x0FF0);
					break;
					case 0x0001:
						this->or_XY(this->fetch_opcode & 0x0FF0);
					break;
					case 0x0002:
						this->and_XY(this->fetch_opcode & 0x0FF0);
					break;
					case 0x0003:
						this->xor_XY(this->fetch_opcode & 0x0FF0);
					break;
					case 0x0004:
						this->add_XY(this->fetch_opcode & 0x0FF0);
					break;
					case 0x0005:
						this->sub_XY(this->fetch_opcode & 0x0FF0);
					break;
					case 0x0006:
						this->shift_right_XY(this->fetch_opcode & 0x0FF0);
					break;
					case 0x0007:
						this->nsub_XY(this->fetch_opcode & 0x0FF0);
					break;
					case 0x000E:
						this->shift_left_XY(this->fetch_opcode & 0x0FF0);
					break;
				};
			};
			break;
			case 0x9000:
				this->cond_x_y_not_eq(this->fetch_opcode & 0x0FF0);
			break;
			case 0xA000:
				this->mem_set_I(this->fetch_opcode & 0x0FFF);
			break;
			case 0xB000:
				this->jump_to_NNNplusV(this->fetch_opcode & 0x0FFF);
			break;
			case 0xC000:
				this->random_bit(this->fetch_opcode & 0x0FFF);
			break;
			case 0xD000:
				this->draw(this->fetch_opcode & 0x0FFF);
			break;
			case 0xE000:
				{
					switch(this->fetch_opcode & 0xFF)
					{
						case 0x9E:
							this->key_press((this->fetch_opcode & 0x0F00) >> 8);
						break;
						case 0xA1:
							this->key_not_press((this->fetch_opcode & 0x0F00) >> 8);
						break;
					}
				};
			break;
			case 0xF000:
			{
				switch (this->fetch_opcode & 0xFF)
				{
					case 0x07:
						this->get_delay_timer((this->fetch_opcode & 0x0F00) >> 8);
					break;
					case 0x0A:
						// we need loop but we can't stop emulator so... PC-=2; and we stay in "wait" loop until some key is finaly pressed.
						this->get_key((this->fetch_opcode & 0x0F00) >> 8);
					break;
					case 0x15:
						this->set_delay_timer((this->fetch_opcode & 0x0F00) >> 8);
					break;
					case 0x18:
						this->set_sound_timer((this->fetch_opcode & 0x0F00) >> 8);
					break;
					case 0x1E:
						this->add_VXtoI((this->fetch_opcode & 0x0F00) >> 8);
					break;
					case 0x29:
						// font sprite at the begining of memory, type * 8 (exp. 2 * 8, A * 8);
						this->set_ItoFontSprite((this->fetch_opcode & 0x0F00) >> 8);
					break;
					case 0x33:
						this->set_BCDofVX((this->fetch_opcode & 0x0F00) >> 8);
					break;
					case 0x55:
					// V0 to VX(included VX)
						this->reg_dump((this->fetch_opcode & 0x0F00) >> 8);
					break;
					case 0x65:
					// V0 to VX(included VX)
						this->reg_load((this->fetch_opcode & 0x0F00) >> 8);
					break;
				};
			};
			break;
		};
	};

	void run()
	{
		this->fetch();
		this->execute();
		this->timer_delay -= this->timer_delay > 0 ? 1: 0;
		this->timer_sound -= this->timer_sound > 0 ? 1: 0;
	};
};
