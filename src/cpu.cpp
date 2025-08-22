

class Components {
public:
	//4096 bytes of memory 0x000 - 0xfff
	unsigned char memory[4096];
	//16 general 8-bit registers. VF is a carry flag
	unsigned char registers[16];

	//DT 8b
	unsigned char DT;

	//ST 8b
	unsigned char ST;

	//index register 16b
	unsigned short index;

	//PC 16bit
	unsigned short PC;

	//SP 8 or 16??
	unsigned char SP;
};



// class Chipsy {
// public:
// 	uint8_t registers[16];
// };