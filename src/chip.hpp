#pragma once

#include "raylib.h"
#include <cstdint>
#include <random>
#include <cstring>
#include <fstream>
#include <iterator>
#include <iostream>
#include <chrono>
#include <random>

const unsigned int MEM_SIZE = 4096;
const unsigned int REG_NUM = 16;
const uint16_t VIDEO_WIDTH = 64;
const uint16_t VIDEO_HEIGHT = 32;

//default colors
// const Color colors[2]{9, 17, 64, 0xFF
// 					,74, 207, 237, 0xFF};
const Color colors[2]{0x29, 0x2b, 0x30, 0xFF
					,0xCF, 0xAB, 0x4A, 0xFF};

class Chip {
public:
	Chip();
	void Cycle();
	void LoadROM(char const* filename);
	
	//void TestDraw();

	uint8_t key[16]{};

	Color video[VIDEO_WIDTH*VIDEO_HEIGHT]{};

private:
	void ftable0();
	void ftable8();
	void ftableE();
	void ftableF();

	void OP_0NNN();
	void OP_00E0(); 
	void OP_00EE();
	void OP_1NNN();
	void OP_2NNN();
	void OP_3XNN();
	void OP_4XNN();
	void OP_5XY0();
	void OP_6XNN();
	void OP_7XNN();
	void OP_8XY0();
	void OP_8XY1();
	void OP_8XY2();
	void OP_8XY3();
	void OP_8XY4();
	void OP_8XY5();
	void OP_8XY6();
	void OP_8XY7();
	void OP_8XYE();
	void OP_9XY0();

	void OP_ANNN();

	void OP_BNNN();
	void OP_CXNN();

	void OP_DXYN(); //draw sprite at vx, vy stored in I

	void OP_EX9E();
	void OP_EXA1();
	void OP_FX07();
	void OP_FX0A();
	void OP_FX15();
	void OP_FX18();
	void OP_FX1E();

	void OP_FX29(uint8_t vx); // set I to address of vx

	void OP_FX33();
	void OP_FX55();
	void OP_FX65();

	//4096 bytes of memory 0x000 - 0xfff
	uint8_t memory[MEM_SIZE]{};
	//16 general 8-bit registers. VF is a carry flag
	uint8_t registers[REG_NUM]{};
	//stack
	uint16_t stack[16]{};
	//DT 8b
	uint8_t dt{};
	//ST 8b
	uint8_t st{};
	//index register 16b
	uint16_t index{};
	//PC 16bit
	uint16_t pc{};
	
	//SP 8 or 16??
	uint8_t sp{};
	uint16_t opcode{};

	std::default_random_engine randGen;
	std::uniform_int_distribution<uint8_t> randByte;


	typedef void (Chip::*PtrTable)();
	PtrTable table[0xF + 1];
	PtrTable table0[0xE + 1];
	PtrTable table8[0xE + 1];
	PtrTable tableE[0xE + 1];
	PtrTable tableF[0x65 + 1];
};