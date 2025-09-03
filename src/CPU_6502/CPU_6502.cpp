#include "CPU_6502.hpp"
#include "../Bus/Bus.hpp"

CPU_6502::CPU_6502(/* args */)
{
    _bus = nullptr;
    _stack_pointer = 0x00;
    _program_counter = 0x0000;
    _x = 0x00;
    _y = 0x00;
    _accumulator_register = 0x00;
    _status_register = 0x00;
    _fetched = 0x00;
    _addr_abs = 0x0000; 
    _addr_relative = 0x0000;
    _opcode = 0x00;
    _cycles = 0x00;

// I am wondering if BRK on the 0,0 place has not as adreessing mode IMP insetead of IMM ????
    using a = CPU_6502;
	_lookup = 
	{
		{ "BRK", &a::BRK, &a::IMM, 7 },{ "ORA", &a::ORA, &a::IZX, 6 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 3 },{ "ORA", &a::ORA, &a::ZP0, 3 },{ "ASL", &a::ASL, &a::ZP0, 5 },{ "???", &a::XXX, &a::IMP, 5 },{ "PHP", &a::PHP, &a::IMP, 3 },{ "ORA", &a::ORA, &a::IMM, 2 },{ "ASL", &a::ASL, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::NOP, &a::IMP, 4 },{ "ORA", &a::ORA, &a::ABS, 4 },{ "ASL", &a::ASL, &a::ABS, 6 },{ "???", &a::XXX, &a::IMP, 6 },
		{ "BPL", &a::BPL, &a::REL, 2 },{ "ORA", &a::ORA, &a::IZY, 5 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 4 },{ "ORA", &a::ORA, &a::ZPX, 4 },{ "ASL", &a::ASL, &a::ZPX, 6 },{ "???", &a::XXX, &a::IMP, 6 },{ "CLC", &a::CLC, &a::IMP, 2 },{ "ORA", &a::ORA, &a::ABY, 4 },{ "???", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 7 },{ "???", &a::NOP, &a::IMP, 4 },{ "ORA", &a::ORA, &a::ABX, 4 },{ "ASL", &a::ASL, &a::ABX, 7 },{ "???", &a::XXX, &a::IMP, 7 },
		{ "JSR", &a::JSR, &a::ABS, 6 },{ "AND", &a::AND, &a::IZX, 6 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "BIT", &a::BIT, &a::ZP0, 3 },{ "AND", &a::AND, &a::ZP0, 3 },{ "ROL", &a::ROL, &a::ZP0, 5 },{ "???", &a::XXX, &a::IMP, 5 },{ "PLP", &a::PLP, &a::IMP, 4 },{ "AND", &a::AND, &a::IMM, 2 },{ "ROL", &a::ROL, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 2 },{ "BIT", &a::BIT, &a::ABS, 4 },{ "AND", &a::AND, &a::ABS, 4 },{ "ROL", &a::ROL, &a::ABS, 6 },{ "???", &a::XXX, &a::IMP, 6 },
		{ "BMI", &a::BMI, &a::REL, 2 },{ "AND", &a::AND, &a::IZY, 5 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 4 },{ "AND", &a::AND, &a::ZPX, 4 },{ "ROL", &a::ROL, &a::ZPX, 6 },{ "???", &a::XXX, &a::IMP, 6 },{ "SEC", &a::SEC, &a::IMP, 2 },{ "AND", &a::AND, &a::ABY, 4 },{ "???", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 7 },{ "???", &a::NOP, &a::IMP, 4 },{ "AND", &a::AND, &a::ABX, 4 },{ "ROL", &a::ROL, &a::ABX, 7 },{ "???", &a::XXX, &a::IMP, 7 },
		{ "RTI", &a::RTI, &a::IMP, 6 },{ "EOR", &a::EOR, &a::IZX, 6 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 3 },{ "EOR", &a::EOR, &a::ZP0, 3 },{ "LSR", &a::LSR, &a::ZP0, 5 },{ "???", &a::XXX, &a::IMP, 5 },{ "PHA", &a::PHA, &a::IMP, 3 },{ "EOR", &a::EOR, &a::IMM, 2 },{ "LSR", &a::LSR, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 2 },{ "JMP", &a::JMP, &a::ABS, 3 },{ "EOR", &a::EOR, &a::ABS, 4 },{ "LSR", &a::LSR, &a::ABS, 6 },{ "???", &a::XXX, &a::IMP, 6 },
		{ "BVC", &a::BVC, &a::REL, 2 },{ "EOR", &a::EOR, &a::IZY, 5 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 4 },{ "EOR", &a::EOR, &a::ZPX, 4 },{ "LSR", &a::LSR, &a::ZPX, 6 },{ "???", &a::XXX, &a::IMP, 6 },{ "CLI", &a::CLI, &a::IMP, 2 },{ "EOR", &a::EOR, &a::ABY, 4 },{ "???", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 7 },{ "???", &a::NOP, &a::IMP, 4 },{ "EOR", &a::EOR, &a::ABX, 4 },{ "LSR", &a::LSR, &a::ABX, 7 },{ "???", &a::XXX, &a::IMP, 7 },
		{ "RTS", &a::RTS, &a::IMP, 6 },{ "ADC", &a::ADC, &a::IZX, 6 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 3 },{ "ADC", &a::ADC, &a::ZP0, 3 },{ "ROR", &a::ROR, &a::ZP0, 5 },{ "???", &a::XXX, &a::IMP, 5 },{ "PLA", &a::PLA, &a::IMP, 4 },{ "ADC", &a::ADC, &a::IMM, 2 },{ "ROR", &a::ROR, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 2 },{ "JMP", &a::JMP, &a::IND, 5 },{ "ADC", &a::ADC, &a::ABS, 4 },{ "ROR", &a::ROR, &a::ABS, 6 },{ "???", &a::XXX, &a::IMP, 6 },
		{ "BVS", &a::BVS, &a::REL, 2 },{ "ADC", &a::ADC, &a::IZY, 5 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 4 },{ "ADC", &a::ADC, &a::ZPX, 4 },{ "ROR", &a::ROR, &a::ZPX, 6 },{ "???", &a::XXX, &a::IMP, 6 },{ "SEI", &a::SEI, &a::IMP, 2 },{ "ADC", &a::ADC, &a::ABY, 4 },{ "???", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 7 },{ "???", &a::NOP, &a::IMP, 4 },{ "ADC", &a::ADC, &a::ABX, 4 },{ "ROR", &a::ROR, &a::ABX, 7 },{ "???", &a::XXX, &a::IMP, 7 },
		{ "???", &a::NOP, &a::IMP, 2 },{ "STA", &a::STA, &a::IZX, 6 },{ "???", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 6 },{ "STY", &a::STY, &a::ZP0, 3 },{ "STA", &a::STA, &a::ZP0, 3 },{ "STX", &a::STX, &a::ZP0, 3 },{ "???", &a::XXX, &a::IMP, 3 },{ "DEY", &a::DEY, &a::IMP, 2 },{ "???", &a::NOP, &a::IMP, 2 },{ "TXA", &a::TXA, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 2 },{ "STY", &a::STY, &a::ABS, 4 },{ "STA", &a::STA, &a::ABS, 4 },{ "STX", &a::STX, &a::ABS, 4 },{ "???", &a::XXX, &a::IMP, 4 },
		{ "BCC", &a::BCC, &a::REL, 2 },{ "STA", &a::STA, &a::IZY, 6 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 6 },{ "STY", &a::STY, &a::ZPX, 4 },{ "STA", &a::STA, &a::ZPX, 4 },{ "STX", &a::STX, &a::ZPY, 4 },{ "???", &a::XXX, &a::IMP, 4 },{ "TYA", &a::TYA, &a::IMP, 2 },{ "STA", &a::STA, &a::ABY, 5 },{ "TXS", &a::TXS, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 5 },{ "???", &a::NOP, &a::IMP, 5 },{ "STA", &a::STA, &a::ABX, 5 },{ "???", &a::XXX, &a::IMP, 5 },{ "???", &a::XXX, &a::IMP, 5 },
		{ "LDY", &a::LDY, &a::IMM, 2 },{ "LDA", &a::LDA, &a::IZX, 6 },{ "LDX", &a::LDX, &a::IMM, 2 },{ "???", &a::XXX, &a::IMP, 6 },{ "LDY", &a::LDY, &a::ZP0, 3 },{ "LDA", &a::LDA, &a::ZP0, 3 },{ "LDX", &a::LDX, &a::ZP0, 3 },{ "???", &a::XXX, &a::IMP, 3 },{ "TAY", &a::TAY, &a::IMP, 2 },{ "LDA", &a::LDA, &a::IMM, 2 },{ "TAX", &a::TAX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 2 },{ "LDY", &a::LDY, &a::ABS, 4 },{ "LDA", &a::LDA, &a::ABS, 4 },{ "LDX", &a::LDX, &a::ABS, 4 },{ "???", &a::XXX, &a::IMP, 4 },
		{ "BCS", &a::BCS, &a::REL, 2 },{ "LDA", &a::LDA, &a::IZY, 5 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 5 },{ "LDY", &a::LDY, &a::ZPX, 4 },{ "LDA", &a::LDA, &a::ZPX, 4 },{ "LDX", &a::LDX, &a::ZPY, 4 },{ "???", &a::XXX, &a::IMP, 4 },{ "CLV", &a::CLV, &a::IMP, 2 },{ "LDA", &a::LDA, &a::ABY, 4 },{ "TSX", &a::TSX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 4 },{ "LDY", &a::LDY, &a::ABX, 4 },{ "LDA", &a::LDA, &a::ABX, 4 },{ "LDX", &a::LDX, &a::ABY, 4 },{ "???", &a::XXX, &a::IMP, 4 },
		{ "CPY", &a::CPY, &a::IMM, 2 },{ "CMP", &a::CMP, &a::IZX, 6 },{ "???", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "CPY", &a::CPY, &a::ZP0, 3 },{ "CMP", &a::CMP, &a::ZP0, 3 },{ "DEC", &a::DEC, &a::ZP0, 5 },{ "???", &a::XXX, &a::IMP, 5 },{ "INY", &a::INY, &a::IMP, 2 },{ "CMP", &a::CMP, &a::IMM, 2 },{ "DEX", &a::DEX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 2 },{ "CPY", &a::CPY, &a::ABS, 4 },{ "CMP", &a::CMP, &a::ABS, 4 },{ "DEC", &a::DEC, &a::ABS, 6 },{ "???", &a::XXX, &a::IMP, 6 },
		{ "BNE", &a::BNE, &a::REL, 2 },{ "CMP", &a::CMP, &a::IZY, 5 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 4 },{ "CMP", &a::CMP, &a::ZPX, 4 },{ "DEC", &a::DEC, &a::ZPX, 6 },{ "???", &a::XXX, &a::IMP, 6 },{ "CLD", &a::CLD, &a::IMP, 2 },{ "CMP", &a::CMP, &a::ABY, 4 },{ "NOP", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 7 },{ "???", &a::NOP, &a::IMP, 4 },{ "CMP", &a::CMP, &a::ABX, 4 },{ "DEC", &a::DEC, &a::ABX, 7 },{ "???", &a::XXX, &a::IMP, 7 },
		{ "CPX", &a::CPX, &a::IMM, 2 },{ "SBC", &a::SBC, &a::IZX, 6 },{ "???", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "CPX", &a::CPX, &a::ZP0, 3 },{ "SBC", &a::SBC, &a::ZP0, 3 },{ "INC", &a::INC, &a::ZP0, 5 },{ "???", &a::XXX, &a::IMP, 5 },{ "INX", &a::INX, &a::IMP, 2 },{ "SBC", &a::SBC, &a::IMM, 2 },{ "NOP", &a::NOP, &a::IMP, 2 },{ "???", &a::SBC, &a::IMP, 2 },{ "CPX", &a::CPX, &a::ABS, 4 },{ "SBC", &a::SBC, &a::ABS, 4 },{ "INC", &a::INC, &a::ABS, 6 },{ "???", &a::XXX, &a::IMP, 6 },
		{ "BEQ", &a::BEQ, &a::REL, 2 },{ "SBC", &a::SBC, &a::IZY, 5 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 4 },{ "SBC", &a::SBC, &a::ZPX, 4 },{ "INC", &a::INC, &a::ZPX, 6 },{ "???", &a::XXX, &a::IMP, 6 },{ "SED", &a::SED, &a::IMP, 2 },{ "SBC", &a::SBC, &a::ABY, 4 },{ "NOP", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 7 },{ "???", &a::NOP, &a::IMP, 4 },{ "SBC", &a::SBC, &a::ABX, 4 },{ "INC", &a::INC, &a::ABX, 7 },{ "???", &a::XXX, &a::IMP, 7 },
	};
}

CPU_6502::~CPU_6502()
{
}


void CPU_6502::ConnectBus(Bus *bus)
{
    _bus = bus;
}

void CPU_6502::write(uint16_t addr, uint8_t data)
{
    _bus->write(addr, data);
}

uint8_t CPU_6502::read(uint16_t addr)
{
    return _bus->read(addr);
}


void CPU_6502::clock()
{
	// here we will just act when the cycle counter is 0
	if (_cycles == 0) {
		// we read what is stored at the PC (program counter). It is the opcode we will execute.
		// this opcode (1 bytes) will serve to index the opcode table we have
		_opcode = this->read(_program_counter);
		// Then we increase the Program counter to be ready to read the next instruction
		_program_counter++;
		// here we put the number of cycle of the opcode in our internal cycle counter
		_cycles = _lookup[_opcode].cycles;

		// the function of the opcode return 0 or 1 indicating if it needs  another clock cycle to continue.
		// Indeed if you look at the doc, you will see some of the instructions on the clock cycles need have a little curve on the number
		// It indicates they could need additionnal cycle to execute. So We will catch those additional needs if necessay.
		// So we will catch it and ruu

		uint8_t additionnal_cycle1 = (this->*_lookup[_opcode].addrmode)();
		uint8_t additionnal_cycle2 = (this->*_lookup[_opcode].operate)();

		// if both addressmode function and operate function indicate they need additional clock cycles, We are going to add them to the cycles counter

		_cycles += (additionnal_cycle1 & additionnal_cycle2);

	}
	// each time we call the clock function, one cycle has been counted
	_cycles--;
}

/// @brief Set the flag f is v is true by updating status register
/// @param f flag to set
/// @param v Bool
void CPU_6502::Setflag(FLAGS6502 f, bool v)
{
	// if the conditionn is validated we set the flag f
	if (v)
		_status_register |= f;
	else
		_status_register &= ~f;
}

// addressing modes

/// @brief There is actually no data part of this instruction
/// @return 0 because doesn't need anything additionnal
uint8_t CPU_6502::IMP()
{
	// however, implied also means that it could be operating upon the accumulator.
	// So we put the content of the accumulator into fetch var
	_fetched = _accumulator_register;
	return 0;
}

/// @brief Immidiate mode addressing means the data supply part of the rest of the instruction. The data (parameter) is going to be the next byte
/// So We will store the actual pc adress (because pc has been incremented) as the address of the data to be read and then increase pc for it to
/// point on the next instruction
/// @return 
uint8_t CPU_6502::IMM()
{

	_addr_abs = _program_counter;

	_program_counter++;

	return 0;

}

/// @brief Zero Page addressing
/// @return 

// pages are a conceptual way of organizing memory.
/* It requires 16 bits to work.
	Let's take the address 0x2315. It is a 16 bits Address. We can seperate it into 2 8 bits (1 byte) number. 23 and 15.
	Here the first 1 byte number (8 bits) indicate the memory page where the information is storage and the second byte (8bits) indicates the offset.
	Just Like a Table.

	So the max is FF (256) pages (Because 00 is a page) and for each page the offset max is FF (256).
	So when The adressing mode ZP0 is called, it means that, the absolute adress of the parameter is just on the read(program counter + 1) Offset 
	of The 0 page. It can happend when developpers want to optimize process
	So we read the 1 byte (2 bits) number on the next program counter memory space (let's call it var) and the absolute address will be 
	absolute_adress = 0x00FF &= var (it will pack the 2bits at the end of 0x00) 

*/
uint8_t CPU_6502::ZP0()
{
	_addr_abs = read(_program_counter);
	_addr_abs &= 0x00FF;
	_program_counter+= 1;

	return 0;

}
/// @brief Zero page register with an offset of X
/// @return 

/*
	The same as ZP0 but here we read the content at program_counter + the_content_of_register_X
	It cpimd be used to iterate through an array

*/
uint8_t CPU_6502::ZPX()
{
	_addr_abs = read(_program_counter + _x);
	_addr_abs &= 0x00FF;
	_program_counter+= 1;

	return 0;
}

/// @brief Zero page register with an offset of X
/// @return 

/*
	The same as ZP0 but here we read the content at program_counter + the_content_of_register_y
	It cpimd be used to iterate through an array

*/
uint8_t CPU_6502::ZPY()
{
	_addr_abs = read(_program_counter + _y);
	_addr_abs &= 0x00FF;
	_program_counter+= 1;

	return 0;
}


/// @brief Absolute addressing mode. Sometime, we need the full adress.
/// But we can only store 8 bits info although an addr is 16 bits. So we will read the info on the program_counter place and the following one
/// Then concat these 8 bits data to have a 16 bits data
/// @return 
uint8_t CPU_6502::ABS()
{
	uint16_t low = this->read(_program_counter);
	_program_counter ++;
	uint16_t high = this->read(_program_counter);
	_program_counter ++;

	// we concate the two 8 bits to have a 16 bits data
	_addr_abs = (high << 8) | low;
	return 0;
}


/// @brief Absolute addressing mode with offset X
/// @return 
uint8_t CPU_6502::ABX()
{
	uint16_t low = this->read(_program_counter);
	_program_counter ++;
	uint16_t high = this->read(_program_counter);
	_program_counter ++;

	// we concate the two 8 bits to have a 16 bits data
	_addr_abs = (high << 8) | low;

	_addr_abs += _x;

	/* 
		Be carefull !!
		If after the add of x register content, the address has change its page, it may need we could need another cycle
	*/

	if ((_addr_abs & 0xFF00) != (high << 8))
		return 1;
	return 0;

}

/// @brief Absolute addressing mode with offset y
/// @return 
uint8_t CPU_6502::ABY()
{
	uint16_t low = this->read(_program_counter);
	_program_counter ++;
	uint16_t high = this->read(_program_counter);
	_program_counter ++;

	// we concate the two 8 bits to have a 16 bits data
	_addr_abs = (high << 8) | low;

	_addr_abs += _y;

	/* 
		Be carefull !!
		If after the add of x register content, the address has change its page, it may need we could need another cycle
	*/

	if ((_addr_abs & 0xFF00) != (high << 8))
		return 1;
	return 0;

}


/// @brief Indirect Mode. It is the 6502 way to implement pointer
/// @return 

/*
	First we assemble the ptr adress by concate the content of the two following memory space on the ram[program_counter]
	Then, we will read what is at this ptr address (1byte) and what is at the ptr + 1 address and we concat them to have the absolute address

	Indeed the param adress is a pointer so we have to see what is at the address of this pointer. The answer has been split into ptr and ptr + 1

	BUT BE CARREFULL. Because we are doing ptr + 1, The page can change and in this case, if the ptr was FFFF, adding a + 1 to the higher part
	OxFF will cause an overflow

	ALSO there was a bug initially in the 6502. Where the low was FF, the ptr Didn't add + 1 to the higher part. We will implement these bug
*/

uint8_t CPU_6502::IND()
{
	uint16_t ptr_low = this->read(_program_counter);
	_program_counter ++;
	uint16_t ptr_high = this->read(_program_counter);
	_program_counter ++;

	uint16_t ptr = (ptr_high << 8) | ptr_low;

	if (ptr_low == 0xFF) // the bug we implemented
		_addr_abs = (this->read(ptr & 0xFF00 ) << 8) | (this->read(ptr + 0));
	else // normal behavior
		_addr_abs = (this->read(ptr + 1 ) << 8) | (this->read(ptr + 0));

	return 0;

}


/// @brief Indirect Addressing of the zero page with an offset of X
/// @return 
uint8_t CPU_6502::INX()
{
	uint16_t t = this->read(_program_counter);
	_program_counter ++;

	// we supply the content of the ram by the content of the x register

	uint16_t ptr_low = this->read((uint16_t) (t + (uint16_t)_x) & 0x00FF);
	uint16_t ptr_high = this->read((uint16_t) (t + (uint16_t)_x + 1) & 0x00FF);

	uint16_t ptr = (ptr_high << 8) | ptr_low;

	if (ptr_low == 0xFF) // the bug we implemented
		_addr_abs = (this->read(ptr & 0xFF00 ) << 8) | (this->read(ptr + 0));
	else // normal behavior
		_addr_abs = (this->read(ptr + 1 ) << 8) | (this->read(ptr + 0));

	return 0;

}

/// @brief Indirect Addressing of the zero page with an offset of y BUT IT IS QUITE DIFFERENT WITH THE INX
/// @return 
uint8_t CPU_6502::INY()
{
	uint16_t t = this->read(_program_counter);
	_program_counter++;


	uint16_t ptr_low = this->read(t & 0x00FF);
	uint16_t ptr_high = this->read((t + 1) & 0x00FF);

	_addr_abs = (ptr_high << 8) | ptr_low;
	_addr_abs += _y;

	if (ptr_high << 8 != _addr_abs & 0XFF00) 
		return 1;

	return 0;

}

/// @brief Relative addressing mode. It only happens to branch instructions
/// @return 
uint8_t CPU_6502::REL()
{

	// for doc check https://youtu.be/8XmxKPJDGU0?si=0CuBBEGAj5vJPN2N&t=2275
	_addr_relative = this->read(_program_counter);
	_program_counter++;

	if (_addr_relative & 0x80)
		_addr_relative |= 0xFF00;
	return 0;

}

// instructions

/// @brief When we know where to read the supply data (parameter), it is time to fetch it and store it
/// @return 
uint8_t CPU_6502::fetch()
{

	// if it is Implied mode, there is no data to fetch
	if (! (_lookup[_opcode].addrmode == &CPU_6502::IMP))
		_fetched = read(_addr_abs);
	return _fetched;
}


/// @brief And between fetcehd data and accumulator register
/// @return 
uint8_t CPU_6502::AND()
{
	fetch();
	_accumulator_register &= _fetched;
	//if all bytes are equal to zero, we set the ZERO flag
	Setflag(Z, _accumulator_register == 0x00);
	// Negative flag is bit 7 is equal to 1
	Setflag(N, _accumulator_register & 0x80);
	return 1; // because this instruction can require another clock cycle
}


/// @brief Branch if the carried register status is set to 1
/// @return 
uint8_t CPU_6502::BCS()
{
	if (GetFlag(C) == 1) {
	 _cycles++;
	// branch instructions are unique and will directly modify the number of cycles
	// When a branch is taken, it will add 1 to the total of cycles required for that intructions
	_addr_abs = _program_counter + _addr_relative;
	// and if a branch have to cross a page of boundarie we add one additionnal cycle "penalty" It is in a doc
	 if ((_addr_abs & 0xFF00) != (_program_counter & 0XFF00))
		 _cycles++;
	// I set the current programm addr by the newly calculated addr
	 _program_counter = _addr_abs;
	}
	return 0;
}


/// @brief Branch if the carried register status is set to 0
/// @return 
uint8_t CPU_6502::BCC()
{
	if (GetFlag(C) == 0) {
	 _cycles++;
	// branch instructions are unique and will directly modify the number of cycles
	// When a branch is taken, it will add 1 to the total of cycles required for that intructions
	_addr_abs = _program_counter + _addr_relative;
	// and if a branch have to cross a page of boundarie we add one additionnal cycle "penalty" It is in a doc
	 if ((_addr_abs & 0xFF00) != (_program_counter & 0XFF00))
		 _cycles++;
	// I set the current programm addr by the newly calculated addr
	 _program_counter = _addr_abs;
	}
	return 0;
}

/// @brief Branch if the carried equal 
/// @return 
uint8_t CPU_6502::BEQ()
{
	if (GetFlag(Z) == 1) {
	 _cycles++;
	// branch instructions are unique and will directly modify the number of cycles
	// When a branch is taken, it will add 1 to the total of cycles required for that intructions
	_addr_abs = _program_counter + _addr_relative;
	// and if a branch have to cross a page of boundarie we add one additionnal cycle "penalty" It is in a doc
	 if ((_addr_abs & 0xFF00) != (_program_counter & 0XFF00))
		 _cycles++;
	// I set the current programm addr by the newly calculated addr
	 _program_counter = _addr_abs;
	}
	return 0;
}


/// @brief Branch if Not equal
/// @return 
uint8_t CPU_6502::BNE()
{
	if (GetFlag(Z) == 0) {
	 _cycles++;
	// branch instructions are unique and will directly modify the number of cycles
	// When a branch is taken, it will add 1 to the total of cycles required for that intructions
	_addr_abs = _program_counter + _addr_relative;
	// and if a branch have to cross a page of boundarie we add one additionnal cycle "penalty" It is in a doc
	 if ((_addr_abs & 0xFF00) != (_program_counter & 0XFF00))
		 _cycles++;
	// I set the current programm addr by the newly calculated addr
	 _program_counter = _addr_abs;
	}
	return 0;
}


/// @brief Branch if negative
/// @return 
uint8_t CPU_6502::BMI()
{
	if (GetFlag(N) == 1) {
	 _cycles++;
	// branch instructions are unique and will directly modify the number of cycles
	// When a branch is taken, it will add 1 to the total of cycles required for that intructions
	_addr_abs = _program_counter + _addr_relative;
	// and if a branch have to cross a page of boundarie we add one additionnal cycle "penalty" It is in a doc
	 if ((_addr_abs & 0xFF00) != (_program_counter & 0XFF00))
		 _cycles++;
	// I set the current programm addr by the newly calculated addr
	 _program_counter = _addr_abs;
	}
	return 0;
}

/// @brief Branch if Positive
/// @return 
uint8_t CPU_6502::BPL()
{
	if (GetFlag(N) == 0) {
	 _cycles++;
	// branch instructions are unique and will directly modify the number of cycles
	// When a branch is taken, it will add 1 to the total of cycles required for that intructions
	_addr_abs = _program_counter + _addr_relative;
	// and if a branch have to cross a page of boundarie we add one additionnal cycle "penalty" It is in a doc
	 if ((_addr_abs & 0xFF00) != (_program_counter & 0XFF00))
		 _cycles++;
	// I set the current programm addr by the newly calculated addr
	 _program_counter = _addr_abs;
	}
	return 0;
}

/// @brief Branch if Overflow
/// @return 
uint8_t CPU_6502::BVC()
{
	if (GetFlag(V) == 0) {
	 _cycles++;
	// branch instructions are unique and will directly modify the number of cycles
	// When a branch is taken, it will add 1 to the total of cycles required for that intructions
	_addr_abs = _program_counter + _addr_relative;
	// and if a branch have to cross a page of boundarie we add one additionnal cycle "penalty" It is in a doc
	 if ((_addr_abs & 0xFF00) != (_program_counter & 0XFF00))
		 _cycles++;
	// I set the current programm addr by the newly calculated addr
	 _program_counter = _addr_abs;
	}
	return 0;
}

/// @brief Branch if not overflow
/// @return 
uint8_t CPU_6502::BVS()
{
	if (GetFlag(V) == 1) {
	 _cycles++;
	// branch instructions are unique and will directly modify the number of cycles
	// When a branch is taken, it will add 1 to the total of cycles required for that intructions
	_addr_abs = _program_counter + _addr_relative;
	// and if a branch have to cross a page of boundarie we add one additionnal cycle "penalty" It is in a doc
	 if ((_addr_abs & 0xFF00) != (_program_counter & 0XFF00))
		 _cycles++;
	// I set the current programm addr by the newly calculated addr
	 _program_counter = _addr_abs;
	}
	return 0;
}


/// @brief Clear the carry bit. Set the bit in the status register
/// @return 
uint8_t CPU_6502::CLC()
{
	Setflag(C, false);
	return 0;
}


/// @brief Clear Decimal Flag
/// @return 
uint8_t CPU_6502::CLD()
{
	Setflag(D, false);
	return 0;
}


/// @brief Clear Overflow Flag
/// @return 
uint8_t CPU_6502::CLV()
{
	Setflag(V, false);
	return 0;
}


/// @brief Disable Interrupts / Clear Interrupt Flag
/// @return 
uint8_t CPU_6502::CLI()
{
	Setflag(I, false);
	return 0;
}