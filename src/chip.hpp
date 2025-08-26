#pragma once

#include <raylib.h>
#include <cstdint>
#include <random>

const unsigned int VIDEO_WIDTH = 64;
const unsigned int VIDEO_HEIGHT = 32;

const Color colors[2]{0, 33, 66, 255
					,10, 207, 190, 255};

class Chip {
public:
	Chip();
	void LoadROM(char const* filename);
	void TestDraw();

	uint8_t key[16]{};
	//uint32_t video[64*32]{};
	// what if my video was a Color* 
	Color video[VIDEO_WIDTH*VIDEO_HEIGHT]{};

	
	

	// const uint32_t SCREENWIDTH{64};
	// const uint32_t SCREENHEIGHT{32};
private:

	void OP_00E0(); 
	void OP_ANNN(uint16_t address);
	void OP_DXYN(uint8_t x, uint8_t y, uint8_t numbytes); //draw sprite at vx, vy stored in I
	void OP_FX29(uint8_t vx); // set I to address of vx

	//4096 bytes of memory 0x000 - 0xfff
	uint8_t memory[4096]{};
	//16 general 8-bit registers. VF is a carry flag
	uint8_t registers[16]{};
	//DT 8b
	uint8_t dt{};
	//ST 8b
	uint8_t st{};
	//index register 16b
	uint16_t index{};
	//PC 16bit
	uint16_t pc{};
	//stack
	uint16_t stack[16]{};
	//SP 8 or 16??
	uint8_t sp{};
	
	uint16_t opcode{};

	std::default_random_engine randGen;
	std::uniform_int_distribution<unsigned char> randByte;
};