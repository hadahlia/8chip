#include "chip.hpp"
#include "raylib.h"

#include <cstdint>
#include <cstring>
#include <fstream>
#include <chrono>
#include <random>

const uint16_t FONTSET_SIZE = 0x50;

const uint16_t START_ADDR = 0x200;

uint8_t fontset[FONTSET_SIZE] = {
	0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
	0x20, 0x60, 0x20, 0x20, 0x70, // 1
	0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
	0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
	0x90, 0x90, 0xF0, 0x10, 0x10, // 4
	0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
	0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
	0xF0, 0x10, 0x20, 0x40, 0x40, // 7
	0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
	0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
	0xF0, 0x90, 0xF0, 0x90, 0x90, // A
	0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
	0xF0, 0x80, 0x80, 0x80, 0xF0, // C
	0xE0, 0x90, 0x90, 0x90, 0xE0, // D
	0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
	0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};

// lil guy
// 0 0 1 0 0 0 1 1 /// 0x23
// 0 1 1 0 0 1 1 1 /// 0x67
// 0 1 0 1 1 0 1 1 /// 0x5A
// 0 1 1 1 1 1 1 0 /// 0x7E
// 0 0 1 1 1 1 0 0 /// 0x3C
uint8_t lilguy[5] = {
	0x23, 0x67, 0x5A, 0x7E, 0x3C
};

Chip::Chip() 
		: randGen(std::chrono::system_clock::now().time_since_epoch().count())
{

	
	pc = START_ADDR;
	
	//fontset into mem
	for (unsigned int i = 0x0; i < FONTSET_SIZE; ++i) {
		memory[i]= fontset[i];
	}

	//lilguy into mem
	for (unsigned int i = 0x0; i < 0x5; ++i) {
		memory[START_ADDR+i] = lilguy[i];
	}
	

	//rng handling
	randByte = std::uniform_int_distribution<uint8_t>(0, 255U);


	// TestDraw();
}

void Chip::LoadROM(char const* filename) {
	std::ifstream file(filename, std::ios::binary | std::ios::ate);

	if (file.is_open()) {
		std::streampos size = file.tellg();

		char* buf = new char[size];

		file.seekg(0, std::ios::beg);
		file.read(buf, size);
		file.close();

		for(long i = 0; i < size; ++i) {
			memory[START_ADDR+i] = buf[i];
		}

		delete[] buf;
	}
}

void Chip::TestDraw() {
	//Chip::OP_FX29()
	Chip::OP_ANNN(0x200);
	Chip::OP_DXYN(0x1A, 0x10, 5);

	//return colorArray;
}

//OPCODES

void Chip::OP_00E0() {
	memset(video, 0, sizeof(video));
}

void Chip::OP_ANNN(uint16_t address) {
	index = address;
}

void Chip::OP_DXYN(uint8_t x, uint8_t y, uint8_t numbytes) {
	

	uint8_t height = numbytes & 0x000Fu;

	uint8_t xpos = x % VIDEO_WIDTH;
	uint8_t ypos = y % VIDEO_HEIGHT;
	// //for (uint8_t )
	// for(unsigned int p = 0; p < 64U*42U; ++p) {
	// 	video[p] = colors[0];
	// }
	
	// video[x+(y)] = colors[1];

	//heres what i want. we can set video[x*y] to what is stored in memory[index]
	for(unsigned int row = 0; row < height; ++row) {
		//uint8_t true_row = row / 8;
		uint8_t spritebyte = memory[index+row];
		
	// 	//video[x+((y+row)*64)] = colors[1];
		for (uint8_t column = 0; column < 8; ++column) {
			uint8_t spritePixel = spritebyte & (0x80u >> column);

			video[(ypos + row)* VIDEO_WIDTH + (xpos + column)] = colors[1];
			Color* screenPixel = &video[(ypos + row)* VIDEO_WIDTH + (xpos + column)];
			//spritebyte = spritebyte << column;

			//spritebyte = spritebyte >> 7;
			//std::printf("byte: %d column: %d", spritebyte & 0b1, column);
			//if ((spritebyte ) == 0b1) {
				//std::printf("", spritebyte);
			//video[(x+column)+((y+row)*64)] = colors[];
			//} else {
			//	video[(x+column)+((y+row)*64)] = colors[0];
		}
			
	}

}

void Chip::OP_FX29(uint8_t vx) {
	index = (uint16_t)vx;
}