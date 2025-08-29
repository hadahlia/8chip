#include "chip.hpp"



const uint16_t FONTSET_SIZE = 80;
const uint16_t FONTSET_START_ADDR = 0x50;
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
// uint8_t lilguy[5] = {
// 	0x23, 0x67, 0x5A, 0x7E, 0x3C
// };

Chip::Chip() 
		: randGen(std::chrono::system_clock::now().time_since_epoch().count())
{

	// memset(memory, 0, sizeof(memory));
	// memset(registers, 0, sizeof(registers));
	// memset(stack, 0, sizeof(stack));
	// memset(video, 0, sizeof(video));
	
	//

	//opcode = 0;
	// dt = 0;
	// st = 0;
	pc = START_ADDR;
	index = 0;

	

	//std::cout << std::hex << (pc ) << "\n";
	
	//fontset into mem
	for (uint16_t i = 0; i < FONTSET_SIZE; ++i) {
		memory[i]= fontset[i];
	}

	//lilguy into mem
	// for (unsigned int i = 0x0; i < 0x5; ++i) {
	// 	memory[START_ADDR+i] = lilguy[i];
	// }
	

	//rng handling
	randByte = std::uniform_int_distribution<uint8_t>(0, 255U);


	table[0x0] = &Chip::ftable0; // todo
	table[0x1] = &Chip::OP_1NNN;
	table[0x2] = &Chip::OP_2NNN;
	table[0x3] = &Chip::OP_3XNN;
	table[0x4] = &Chip::OP_4XNN;
	table[0x5] = &Chip::OP_5XY0;
	table[0x6] = &Chip::OP_6XNN;
	table[0x7] = &Chip::OP_7XNN;
	table[0x8] = &Chip::ftable8; // todo
	table[0x9] = &Chip::OP_9XY0;
	table[0xA] = &Chip::OP_ANNN;
	table[0xB] = &Chip::OP_BNNN;
	table[0xC] = &Chip::OP_CXNN;
	table[0xD] = &Chip::OP_DXYN;
	table[0xE] = &Chip::ftableE; //todo
	table[0xF] = &Chip::ftableF; //todo. christ

	table0[0x0] = &Chip::OP_00E0;
	table0[0xE] = &Chip::OP_00EE;
	// tableE[0x1] = &Chip::OP_EXA1;
	// tableE[0xE] = &Chip::OP_EX9E;

	// TestDraw();
}

void Chip::LoadROM(char const* filename) {
	//if (!filename) {return;}
	// std::ifstream file(filename, std::ios::binary | std::ios::ate);

	std::ifstream file(filename, std::ios::binary);

	//if (file.is_open()) {

	//std::streampos size = file.tellg();
	//if (size == 0) { return; }
	//unsigned char* buf = new unsigned char[size];

	std::vector<uint8_t> buf(std::istreambuf_iterator<char>(file), {});

	// file.seekg(0, std::ios::beg);
	// file.read(buf, size);
	// file.close();

	// uint8_t tmp[buf.size()];
	// std::copy(buf.begin(), buf.end(), tmp);

	for(uint16_t i = 0; i < buf.size(); ++i) {
		memory[START_ADDR+i] = buf[i];

		//std::cout << std::hex << memory[START_ADDR+i] << std::endl;
	}

	//std::cout << std::hex << memory[pc] << std::endl;
	// for(const uint8_t& i : buf) {
	// 	// memory[START_ADDR+i] = buf[i];
	// 	memory[START_ADDR] = i;
	// 	//std::cout << std::format("{:b}") << buf[i] << std::endl;
	// }

	buf.clear();
	buf.shrink_to_fit();
	//delete[] tmp;
	
	//}
	// std::cout << "memory -> " << std::hex << (0x0000u | (memory[0x200] << 8u) | memory[0x200 + 1]);



	// std::cout << " pc -> " << std::hex << this->pc << " opcode -> " << this->opcode <<  std::endl;

	// std::cout << "rom loaded" << std::endl;

	//std::cout << std::hex << memory[pc] << std::endl;
}

void Chip::Cycle() {
	//problem child
	
	opcode = (memory[pc] << 8u) | memory[pc + 1];

	//std::cout << "memory -> " << (0x0000u | (memory[0x200u] << 8u) | memory[0x200u + 1]);
	
	std::cout << "value " << std::hex << ((0x0000 | memory[index]) << 8u | memory[index + 1]) << " at index " << std::hex << index;



	std::cout << " pc -> " << std::hex << pc << " opcode -> " << opcode <<  std::endl;

	std::cout << " every single register value: ";
	for (int i = 0; i < 16; ++i) {
		std::cout << " v" << i << ": " << std::hex << registers[i];
	}
	std::cout << std::endl;

	if (pc < 0x1000u) {
		pc += 2;
	} else {
		pc = 0xFFFu; // shwug
	}
	
	
	((*this).*(table[(opcode & 0xF000u) >> 12u]))();


	if (dt > 0) {
		--dt;
	}

	if(st > 0) {
		--st;
	}


}

//void Chip::TestDraw() {
	//Chip::OP_FX29()
	// Chip::OP_ANNN(0x200);
	// Chip::OP_DXYN(0x1A, 0x10, 5);

	//return colorArray;
//}


void Chip::ftable0() {
	// opcode nibble is 0
	((*this).*(table0[opcode & 0x000Fu]))();
}

void Chip::ftable8() {
	//opcode nibble is 8
}
void Chip::ftableE() {
	//opcode nibble is E
}
void Chip::ftableF() {
	//opcode nibble is F
}

//OPCODES


void Chip::OP_0NNN() {

}

void Chip::OP_00E0() {
	memset(video, 0, sizeof(video));
}



void Chip::OP_00EE() {
	--sp;
	pc = stack[sp];
}


void Chip::OP_1NNN() {
	pc = opcode & 0xFFFu;
}


void Chip::OP_2NNN() {
	stack[sp] = pc;
	++sp;
	pc = opcode & 0xFFFu;
}


void Chip::OP_3XNN() {
	if (registers[(opcode & 0xF00u) >> 8u] == opcode & 0xFFu) {
		pc += 2;
	}
}


void Chip::OP_4XNN() {
	if (registers[(opcode & 0xF00u) >> 8u] != opcode & 0xFFu) {
		pc += 2;
	}
}


void Chip::OP_5XY0() {
	if (registers[(opcode & 0xF00) >> 8u] == registers[(opcode & 0xF0) >> 4u]) {
		pc += 2;
	}
}


void Chip::OP_6XNN() {

	registers[(opcode & 0xF00u) >> 8u] = opcode & 0xFFu;

}


void Chip::OP_7XNN() {
	registers[(opcode & 0xF00u) >> 8u] += opcode & 0xFFu;
}


void Chip::OP_8XY0() {
	registers[(opcode & 0xF00u) >> 8u] = registers[(opcode & 0xF0) >> 4u];
}


void Chip::OP_8XY1() {

}


void Chip::OP_8XY2() {

}


void Chip::OP_8XY3() {

}


void Chip::OP_8XY4() {

}


void Chip::OP_8XY5() {

}


void Chip::OP_8XY6() {

}


void Chip::OP_8XY7() {

}


void Chip::OP_8XYE() {

}


void Chip::OP_9XY0() {
	if (registers[(opcode & 0xF00u) >> 8u] != registers[(opcode & 0xF0u) >> 4u]) {
		pc += 2;
	}
}


void Chip::OP_ANNN() {
	index = opcode & 0x0FFFu;
}


void Chip::OP_BNNN() {
	pc = (opcode & 0xFFFu) + registers[0];
}


void Chip::OP_CXNN() {
	registers[(opcode & 0xF00u) >> 8u] = randByte(randGen) & (opcode & 0xFFu);
}


void Chip::OP_DXYN() {
	
	uint8_t x = (opcode & 0xF00u) >> 8u;
	uint8_t y  = (opcode & 0xF0u) >> 4u;
	uint8_t height = opcode & 0xFu;

	uint8_t xpos = registers[x] % VIDEO_WIDTH;
	uint8_t ypos = registers[y] % VIDEO_HEIGHT;
	
	for(unsigned int row = 0; row < height; ++row) {
		//uint8_t true_row = row / 8;
		uint8_t spritebyte = memory[index+row];
		
		for (uint8_t column = 0; column < 8; ++column) {
			//uint8_t spritePixel = spritebyte & (0x80u >> column);

			video[(ypos + row)* VIDEO_WIDTH + (xpos + column)] = colors[1];
			//Color* screenPixel = &video[(ypos + row)* VIDEO_WIDTH + (xpos + column)];
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

void Chip::OP_EX9E() {

}


void Chip::OP_EXA1() {

}


void Chip::OP_FX07() {

}


void Chip::OP_FX0A() {

}


void Chip::OP_FX15() {

}


void Chip::OP_FX18() {

}


void Chip::OP_FX1E() {

}


void Chip::OP_FX29(uint8_t vx) {
	index = (uint16_t)vx;
}


void Chip::OP_FX33() {

}


void Chip::OP_FX55() {

}


void Chip::OP_FX65() {

}

