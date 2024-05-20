#include <iostream>
#include "chip8_unit.cpp"
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <random>
#include <fstream>

int rand_number()
{
	std::rand();
	std::random_device rd;
	std::mt19937 gen(rd());

	std::uniform_int_distribution<int> distro(0, 255);

	return distro(gen);
}

void copy_vram_to_image(uint8_t* memory, uint32_t* image_buffer)
{
	for (int16_t i=0; i<0x100; i++)
	{
		for (int16_t bit=0; bit < 8; bit++)
		{
			image_buffer[i*8 + (7 - bit)] = (((memory[i] >> (7 - bit))&1) == 1 ? 0xFFFFFFFF : 0xFF000000);
//			printf("%i ", ((memory[i] >> (7-bit))&1));
//			printf("%i ", (i*8 + (7-bit)));
		}
//		printf("\n");
	}


}

void load_file(char* path, uint8_t* memory)
{
	uint16_t start_address = 0x200;

	std::ifstream file(path, std::ios::binary | std::ios::ate);

	if(file.is_open())
	{
		std::streampos size = file.tellg();
		char* buffer = new char[size];

		file.seekg(0,std::ios::beg);
		file.read(buffer, size);
		file.close();

		for (uint16_t i=0;i<size;i++)
		{
			memory[start_address + i] = buffer[i];
		}

		delete[] buffer;
	}

}

int main(int arg_count, char** arg_vec)
{
    if(arg_count < 2)
    {
	printf("NOT ENOUGH ARGUMENTS\nsample: ./chipemu 'filename/path'\n");
	return -1000;
    }


	int w_width = 640;
	int w_height = 480;

    std::cout<< "Hello chip8emu!!" << std::endl;
	
	CPU_8 chip8;

	chip8.makeClear();

	chip8.b_debug = false;

    unsigned int window_sdl_opengl_flag = SDL_WINDOW_OPENGL;

	if (SDL_Init(SDL_INIT_VIDEO) != 0) 
	{
		printf("SDL_Init Error: %s\n", SDL_GetError());
	       return -1;
	}

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	
	
	SDL_Window* window = SDL_CreateWindow("Hello SDL", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, w_width, w_height, window_sdl_opengl_flag);
	if (window == NULL)	
	{
		printf("SDL_CreateWindow Error: %s\n", SDL_GetError());
		SDL_Quit();
	       return -1;
	}

	SDL_GLContext context_opengl = SDL_GL_CreateContext(window);

	if (!context_opengl)	
	{
		printf("SDL_GL_CreateContext Error: %s\n", SDL_GetError());
		SDL_DestroyWindow(window);
		SDL_Quit();
	       return -1;
	}

	float red = 0, green = 0, blue = 0;
		// R G B A
	glViewport(0, 0, w_width, w_height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glClearColor(red, green, blue, 1.0f);
	
	bool running = true;
	SDL_Event all_events;
	
	// image in bits, size 64x32, 0/1 = off/on pixel
	uint8_t* memory = new uint8_t[0x100]();
	GLuint texture_image;
	uint16_t image_size = 64*32;
	uint32_t* image_buffer = new uint32_t[image_size]();

	
//	image_buffer[0]=0;					
	
//	for (int16_t i = 1; i < 64*32; i++)
//	{
//		image_buffer[i] = i%2 > 0 ? 0xFFFFFFFF : 0xFF000000;
//	}

	glGenTextures(1, &texture_image);
	glBindTexture(GL_TEXTURE_2D, texture_image);

	glTexImage2D(GL_TEXTURE_2D, 0,GL_RGBA, 64, 32, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_buffer);
//	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 64, 32, GL_RGBA, GL_UNSIGNED_BYTE, &image_buffer);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glBindTexture(GL_TEXTURE_2D, 0);

	glClearColor(0.20f, 0.20f, 0.20f, 1.0f);

	SDL_GL_SwapWindow(window);

	uint32_t last_time = SDL_GetTicks();

	uint32_t chip_tick = 0;

	bool b_make_step=false;
	bool b_key_blocked=false;
	bool b_run_emu = true;

	// load rom to chip memory
	load_file(arg_vec[1], chip8.memory);

	while (running)
	{
	
		while (SDL_PollEvent(&all_events) != 0)
		{
			if (all_events.type == SDL_QUIT)
			{
				running = false;
			}
			else if (all_events.type == SDL_KEYDOWN)
			{	
				switch (all_events.key.keysym.sym)
				{
					case SDLK_ESCAPE:
						running =false;
						break;
					case SDLK_F1:
						if(!b_key_blocked)
						{
						b_run_emu = !b_run_emu;
						b_key_blocked = true;
						}
					//	copy_vram_to_image(&chip8.memory[0xF00], image_buffer);
						break;

					case SDLK_0:
						{ // step cycle
							if (!b_key_blocked) b_make_step=true;
							b_key_blocked=true;
						}
						break;

					case SDLK_1:
						chip8.keys[0x1] = 1;
						break;
					case SDLK_2:
						chip8.keys[0x2] = 1;
						break;
					case SDLK_3:
						chip8.keys[0x3] = 1;
						break;
					case SDLK_4:
						chip8.keys[0xC] = 1;
						break;
					case SDLK_q:
						chip8.keys[0x4] = 1;
						break;
					case SDLK_w:
						chip8.keys[0x5] = 1;
						break;
					case SDLK_e:
						chip8.keys[0x6] = 1;
						break;
					case SDLK_r:
						chip8.keys[0xD] = 1;
						break;
					case SDLK_a:
						chip8.keys[0x7] = 1;
						break;
					case SDLK_s:
						chip8.keys[0x8] = 1;
						break;
					case SDLK_d:
						chip8.keys[0x9] = 1;
						break;
					case SDLK_f:
						chip8.keys[0xE] = 1;
						break;
					case SDLK_z:
						chip8.keys[0xA] = 1;
						break;
					case SDLK_x:
						chip8.keys[0x0] = 1;
						break;
					case SDLK_c:
						chip8.keys[0xB] = 1;
						break;
					case SDLK_v:
						chip8.keys[0xF] = 1;
						break;
				}
			}
			else if (all_events.type == SDL_KEYUP)
			{	
				switch (all_events.key.keysym.sym)
				{
					case SDLK_0: // step cycle
						b_key_blocked=false;
						break;

					case SDLK_1:
						chip8.keys[0x1] = 0;
						break;
					case SDLK_2:
						chip8.keys[0x2] = 0;
						break;
					case SDLK_3:
						chip8.keys[0x3] = 0;
						break;
					case SDLK_4:
						chip8.keys[0xC] = 0;
						break;
					case SDLK_q:
						chip8.keys[0x4] = 0;
						break;
					case SDLK_w:
						chip8.keys[0x5] = 0;
						break;
					case SDLK_e:
						chip8.keys[0x6] = 0;
						break;
					case SDLK_r:
						chip8.keys[0xD] = 0;
						break;
					case SDLK_a:
						chip8.keys[0x7] = 0;
						break;
					case SDLK_s:
						chip8.keys[0x8] = 0;
						break;
					case SDLK_d:
						chip8.keys[0x9] = 0;
						break;
					case SDLK_f:
						chip8.keys[0xE] = 0;
						break;
					case SDLK_z:
						chip8.keys[0xA] = 0;
						break;
					case SDLK_x:
						chip8.keys[0x0] = 0;
						break;
					case SDLK_c:
						chip8.keys[0xB] = 0;
						break;
					case SDLK_v:
						chip8.keys[0xF] = 0;
						break;
				}
			}
		}
	
		glClear(GL_COLOR_BUFFER_BIT);
	if(chip8.draw_flag)
	{
		copy_vram_to_image(&chip8.memory[0xF00], image_buffer);
		chip8.draw_flag = false;
	}	
	
	//position
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture_image);
	glTexImage2D(GL_TEXTURE_2D, 0,GL_RGBA, 64, 32, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_buffer);
	glBegin(GL_QUADS);
	
	// image coords
	glTexCoord2f(0.0f, 1.0f);	glVertex2f(-0.50f, -0.50f);
	glTexCoord2f(1.0f, 1.0f);	glVertex2f(0.50f, -0.50f);
	glTexCoord2f(1.0f, 0.0f);	glVertex2f(0.50f, 0.50f);
	glTexCoord2f(0.0f, 0.0f);	glVertex2f(-0.50f, 0.50f);
	
	glEnd();
	glBindTexture(GL_TEXTURE_2D, 0);
	
	glDisable(GL_TEXTURE_2D);
	// chip8 update
	
	if (b_make_step | b_run_emu)
	{
		chip8.run();
		b_make_step=false;
	};

	//

	SDL_GL_SwapWindow(window);

	}

	glDeleteTextures(1, &texture_image);
	SDL_GL_DeleteContext(context_opengl);
	SDL_DestroyWindow(window);
	SDL_Quit();
	
	delete[] image_buffer;
	delete[] memory;

    return 0;
}
