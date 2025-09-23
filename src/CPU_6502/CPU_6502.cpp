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
    std::cout << (int)_status_register << "  l"<<std::endl;
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


// Returns the value of a specific bit of the status register
uint8_t CPU_6502::GetFlag(FLAGS6502 f)
{
	return ((_status_register & f) > 0) ? 1 : 0;
}


/// @brief Set the flag f is v is true by updating status register otherwise unset it
/// @param f flag to set
/// @param v Bool. If true set the flag on the status register. Else Un set it
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


// Address Mode: Indirect X
// The supplied 8-bit address is offset by X Register to index
// a location in page 0x00. The actual 16-bit address is read 
// from this location
uint8_t CPU_6502::IZX()
{
	uint16_t t = read(_program_counter);
	_program_counter++;

	uint16_t lo = read((uint16_t)(t + (uint16_t)_x) & 0x00FF);
	uint16_t hi = read((uint16_t)(t + (uint16_t)_x + 1) & 0x00FF);

	_addr_abs = (hi << 8) | lo;
	
	return 0;
}


// Address Mode: Indirect Y
// The supplied 8-bit address indexes a location in page 0x00. From 
// here the actual 16-bit address is read, and the contents of
// Y Register is added to it to offset it. If the offset causes a
// change in page then an additional clock cycle is required.
uint8_t CPU_6502::IZY()
{
	uint16_t t = read(_program_counter);
	_program_counter++;

	uint16_t lo = read(t & 0x00FF);
	uint16_t hi = read((t + 1) & 0x00FF);

	_addr_abs = (hi << 8) | lo;
	_addr_abs += _y;
	
	if ((_addr_abs & 0xFF00) != (hi << 8))
		return 1;
	else
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


uint8_t CPU_6502::ASL()
{
	fetch();
	uint16_t temp = (uint16_t)_fetched << 1;
	Setflag(C, (temp & 0xFF00) > 0);
	Setflag(Z, (temp & 0x00FF) == 0x00);
	Setflag(N, temp & 0x80);
	if (_lookup[_opcode].addrmode == &CPU_6502::IMP)
		_accumulator_register = temp & 0x00FF;
	else
		write(_addr_abs, temp & 0x00FF);
	return 0;
}

uint8_t CPU_6502::BIT()
{
	fetch();
	uint16_t temp = _accumulator_register & _fetched;
	Setflag(Z, (temp & 0x00FF) == 0x00);
	Setflag(N, _fetched & (1 << 7));
	Setflag(V, _fetched & (1 << 6));
	return 0;
}

uint8_t CPU_6502::BRK()
{
	_program_counter++;
	
	Setflag(I, 1);
	write(0x0100 + _stack_pointer, (_program_counter >> 8) & 0x00FF);
	_stack_pointer--;
	write(0x0100 + _stack_pointer, _program_counter & 0x00FF);
	_stack_pointer--;

	Setflag(B, 1);
	write(0x0100 + _stack_pointer, _status_register);
	_stack_pointer--;
	Setflag(B, 0);

	_program_counter = (uint16_t)read(0xFFFE) | ((uint16_t)read(0xFFFF) << 8);
	return 0;
}

#include "CPU_6502.hpp"
#include "../Bus/Bus.hpp"

uint8_t CPU_6502::CMP()
{
	fetch();
	uint16_t temp = (uint16_t)_accumulator_register - (uint16_t)_fetched;
	Setflag(C, _accumulator_register >= _fetched);
	Setflag(Z, (temp & 0x00FF) == 0x0000);
	Setflag(N, temp & 0x0080);
	return 1;
}


// Instruction: Compare _x Register
// Function:    C <- _x >= M      Z <- (_x - M) == 0
// Flags Out:   N, C, Z
uint8_t CPU_6502::CPX()
{
	fetch();
	uint16_t temp = (uint16_t)_x - (uint16_t)_fetched;
	Setflag(C, _x >= _fetched);
	Setflag(Z, (temp & 0x00FF) == 0x0000);
	Setflag(N, temp & 0x0080);
	return 0;
}


// Instruction: Compare Y Register
// Function:    C <- Y >= M      Z <- (Y - M) == 0
// Flags Out:   N, C, Z
uint8_t CPU_6502::CPY()
{
	fetch();
	uint16_t temp = (uint16_t)_y - (uint16_t)_fetched;
	Setflag(C, _y >= _fetched);
	Setflag(Z, (temp & 0x00FF) == 0x0000);
	Setflag(N, temp & 0x0080);
	return 0;
}

// Instruction: Decrement Value at Memory Location
// Function:    M = M - 1
// Flags Out:   N, Z
uint8_t CPU_6502::DEC()
{
	fetch();
	uint16_t temp = _fetched - 1;
	write(_addr_abs, temp & 0x00FF);
	Setflag(Z, (temp & 0x00FF) == 0x0000);
	Setflag(N, temp & 0x0080);
	return 0;
}


// Instruction: Decrement _x Register
// Function:    _x = _x - 1
// Flags Out:   N, Z
uint8_t CPU_6502::DEX()
{
	_x--;
	Setflag(Z, _x == 0x00);
	Setflag(N, _x & 0x80);
	return 0;
}


// Instruction: Decrement Y Register
// Function:    Y = Y - 1
// Flags Out:   N, Z
uint8_t CPU_6502::DEY()
{
	_y--;
	Setflag(Z, _y == 0x00);
	Setflag(N, _y & 0x80);
	return 0;
}

uint8_t CPU_6502::EOR()
{
	fetch();
	_accumulator_register = _accumulator_register ^ _fetched;	
	Setflag(Z, _accumulator_register == 0x00);
	Setflag(N, _accumulator_register & 0x80);
	return 1;
}


// Instruction: Increment Value at Memory Location
// Function:    M = M + 1
// Flags Out:   N, Z
uint8_t CPU_6502::INC()
{
	fetch();
	uint16_t temp = _fetched + 1;
	write(_addr_abs, temp & 0x00FF);
	Setflag(Z, (temp & 0x00FF) == 0x0000);
	Setflag(N, temp & 0x0080);
	return 0;
}

// Instruction: Jump To Location
// Function:    _program_counter = address
uint8_t CPU_6502::JMP()
{
	_program_counter = _addr_abs;
	return 0;
}


// Instruction: Jump To Sub-Routine
// Function:    Push current _program_counter to stack, _program_counter = address
uint8_t CPU_6502::JSR()
{
	_program_counter--;

	write(0x0100 + _stack_pointer, (_program_counter >> 8) & 0x00FF);
	_stack_pointer--;
	write(0x0100 + _stack_pointer, _program_counter & 0x00FF);
	_stack_pointer--;

	_program_counter = _addr_abs;
	return 0;
}


// Instruction: Load The Accumulator
// Function:    A = M
// Flags Out:   N, Z
uint8_t CPU_6502::LDA()
{
	fetch();
	_accumulator_register = _fetched;
	Setflag(Z, _accumulator_register == 0x00);
	Setflag(N, _accumulator_register & 0x80);
	return 1;
}


// Instruction: Load The _x Register
// Function:    _x = M
// Flags Out:   N, Z
uint8_t CPU_6502::LDX()
{
	fetch();
	_x = _fetched;
	Setflag(Z, _x == 0x00);
	Setflag(N, _x & 0x80);
	return 1;
}


// Instruction: Load The Y Register
// Function:    Y = M
// Flags Out:   N, Z
uint8_t CPU_6502::LDY()
{
	fetch();
	_y = _fetched;
	Setflag(Z, _y == 0x00);
	Setflag(N, _y & 0x80);
	return 1;
}

uint8_t CPU_6502::LSR()
{
	fetch();
	Setflag(C, _fetched & 0x0001);
	uint16_t temp = _fetched >> 1;	
	Setflag(Z, (temp & 0x00FF) == 0x0000);
	Setflag(N, temp & 0x0080);
	if (_lookup[_opcode].addrmode == &CPU_6502::IMP)
		_accumulator_register = temp & 0x00FF;
	else
		write(_addr_abs, temp & 0x00FF);
	return 0;
}

uint8_t CPU_6502::NOP()
{
	// Sadly not all NOPs are equal, Ive added a few here
	// based on https://wiki.nesdev.com/w/index.php/CPU_unofficial__opcodes
	// and will add more based on game compatibility, and ultimately
	// I'd like to cover all illegal _opcodes too
	switch (_opcode) {
	case 0x1C:
	case 0x3C:
	case 0x5C:
	case 0x7C:
	case 0xDC:
	case 0xFC:
		return 1;
		break;
	}
	return 0;
}


// Instruction: Bitwise Logic OR
// Function:    A = A | M
// Flags Out:   N, Z
uint8_t CPU_6502::ORA()
{
	fetch();
	_accumulator_register = _accumulator_register | _fetched;
	Setflag(Z, _accumulator_register == 0x00);
	Setflag(N, _accumulator_register & 0x80);
	return 1;
}

// Instruction: Push Status Register to Stack
// Function:    status -> stack
// Note:        Break flag is set to 1 before push
uint8_t CPU_6502::PHP()
{
	write(0x0100 + _stack_pointer, _status_register | B | U);
	Setflag(B, 0);
	Setflag(U, 0);
	_stack_pointer--;
	return 0;
}

uint8_t CPU_6502::PLP()
{
	_stack_pointer++;
	_status_register = read(0x0100 + _stack_pointer);
	Setflag(U, 1);
	return 0;
}

uint8_t CPU_6502::ROL()
{
	fetch();
	uint16_t temp = (uint16_t)(_fetched << 1) | GetFlag(C);
	Setflag(C, temp & 0xFF00);
	Setflag(Z, (temp & 0x00FF) == 0x0000);
	Setflag(N, temp & 0x0080);
	if (_lookup[_opcode].addrmode == &CPU_6502::IMP)
		_accumulator_register = temp & 0x00FF;
	else
		write(_addr_abs, temp & 0x00FF);
	return 0;
}

uint8_t CPU_6502::ROR()
{
	fetch();
	uint16_t temp = (uint16_t)(GetFlag(C) << 7) | (_fetched >> 1);
	Setflag(C, _fetched & 0x01);
	Setflag(Z, (temp & 0x00FF) == 0x00);
	Setflag(N, temp & 0x0080);
	if (_lookup[_opcode].addrmode == &CPU_6502::IMP)
		_accumulator_register = temp & 0x00FF;
	else
		write(_addr_abs, temp & 0x00FF);
	return 0;
}

uint8_t CPU_6502::RTS()
{
	_stack_pointer++;
	_program_counter = (uint16_t)read(0x0100 + _stack_pointer);
	_stack_pointer++;
	_program_counter |= (uint16_t)read(0x0100 + _stack_pointer) << 8;
	
	_program_counter++;
	return 0;
}




// Instruction: Set Carry Flag
// Function:    C = 1
uint8_t CPU_6502::SEC()
{
	Setflag(C, true);
	return 0;
}


// Instruction: Set Decimal Flag
// Function:    D = 1
uint8_t CPU_6502::SED()
{
	Setflag(D, true);
	return 0;
}


// Instruction: Set Interrupt Flag / Enable Interrupts
// Function:    I = 1
uint8_t CPU_6502::SEI()
{
	Setflag(I, true);
	return 0;
}

// Instruction: Store Accumulator at Address
// Function:    M = A
uint8_t CPU_6502::STA()
{
	write(_addr_abs, _accumulator_register);
	return 0;
}


// Instruction: Store _x Register at Address
// Function:    M = _x
uint8_t CPU_6502::STX()
{
	write(_addr_abs, _x);
	return 0;
}


// Instruction: Store Y Register at Address
// Function:    M = Y
uint8_t CPU_6502::STY()
{
	write(_addr_abs, _y);
	return 0;
}


// Instruction: Transfer Accumulator to _x Register
// Function:    _x = A
// Flags Out:   N, Z
uint8_t CPU_6502::TAX()
{
	_x = _accumulator_register;
	Setflag(Z, _x == 0x00);
	Setflag(N, _x & 0x80);
	return 0;
}


// Instruction: Transfer Accumulator to Y Register
// Function:    Y = A
// Flags Out:   N, Z
uint8_t CPU_6502::TAY()
{
	_y = _accumulator_register;
	Setflag(Z, _y == 0x00);
	Setflag(N, _y & 0x80);
	return 0;
}


// Instruction: Transfer Stack Pointer to _x Register
// Function:    _x = stack pointer
// Flags Out:   N, Z
uint8_t CPU_6502::TSX()
{
	_x = _stack_pointer;
	Setflag(Z, _x == 0x00);
	Setflag(N, _x & 0x80);
	return 0;
}


// Instruction: Transfer _x Register to Accumulator
// Function:    A = _x
// Flags Out:   N, Z
uint8_t CPU_6502::TXA()
{
	_accumulator_register = _x;
	Setflag(Z, _accumulator_register == 0x00);
	Setflag(N, _accumulator_register & 0x80);
	return 0;
}


// Instruction: Transfer _x Register to Stack Pointer
// Function:    stack pointer = _x
uint8_t CPU_6502::TXS()
{
	_stack_pointer = _x;
	return 0;
}


// Instruction: Transfer Y Register to Accumulator
// Function:    A = Y
// Flags Out:   N, Z
uint8_t CPU_6502::TYA()
{
	_accumulator_register = _y;
	Setflag(Z, _accumulator_register == 0x00);
	Setflag(N, _accumulator_register & 0x80);
	return 0;
}


// This function captures illegal _opcodes
uint8_t CPU_6502::XXX()
{
	return 0;
}


uint8_t CPU_6502::ADC()
{
	this->fetch();

	// We perfom the addition under 16 bits. 
	// We do not forget about the varry
	uint16_t temp = (uint16_t) _accumulator_register + (uint16_t)_fetched + (uint16_t)GetFlag(C); 
	// Because the temp is a 16 bits var, we can quickly check if after the operation, we need to set the carry bit
	Setflag(C, temp > 255);
	Setflag(Z, (temp & 0x00FF) == 0);
	// The negative bit is set when the most significant bit of the LOW BYTE of the restult is set
	Setflag(N, temp & 0x80);

	// check out the notes on the read me to understand How We Set the V (Overflow) bit of the status register
	Setflag(V, (~((uint16_t)_accumulator_register ^ (uint16_t)_fetched) & ((uint16_t)_accumulator_register ^ (uint16_t)temp)) & 0x0080);
	
	// Load the result into the accumulator (it's 8-bit dont forget!)
	_accumulator_register = temp & 0x00FF;
	
	// This instruction has the potential to require an additional clock cycle
	return 1;

}

uint8_t CPU_6502::SBC()
{
	this->fetch();
	
	// Operating in 16-bit domain to capture carry out
	
	// We can invert the bottom 8 bits with bitwise xor
	uint16_t value = ((uint16_t)_fetched) ^ 0x00FF;
	
	// Notice this is exactly the same as addition from here!
	uint16_t  temp = (uint16_t)_accumulator_register + value + (uint16_t)GetFlag(C);
	Setflag(C, temp & 0xFF00);
	Setflag(Z, ((temp & 0x00FF) == 0));
	Setflag(V, (temp ^ (uint16_t)_accumulator_register) & (temp ^ value) & 0x0080);
	Setflag(N, temp & 0x0080);
	_accumulator_register = temp & 0x00FF;
	return 1;
}

/// @brief Push the accumulator into the stack and decrease the stack pointer
/// @return 
uint8_t CPU_6502::PHA()
{
	//The 6502 has hardcoded a basic locationfor the stack 0x0100
	this->write(0x0100 + _stack_pointer, _accumulator_register);
	_stack_pointer --;
	return 0;
}


/// @brief Pop data from the stack to the accumulator register
/// @return 
uint8_t CPU_6502::PLA()
{
	_stack_pointer++;
	_accumulator_register = this->read(0X0100 + _stack_pointer);
	// We also set Z and N to status register if accumulator is Z or N
	Setflag(Z, _accumulator_register==0x00);
	Setflag(N, _accumulator_register & 0X80);
	return 0;
}


/// @brief It configure the CPu into a none state
void CPU_6502::reset()
{
	_accumulator_register = 0;
	_x = 0;
	_y = 0;
	_stack_pointer = 0XFD;
	_status_register = 0X00 | U;


	// To set the value of the program counter, we could be tempted to put 0
	// But this value has to be set somewhere in the memory so the programmer can change it
	// without having to compile the whole machine. So the value will be stocked at
	// 0XFFFC and OFFCD and will be concatened to know the starting programe counter
	_addr_abs = 0xFFFC;
	uint16_t lo = read(_addr_abs + 0);
	uint16_t hi = read(_addr_abs + 1);

	_program_counter = (hi << 8) | lo;

	_addr_relative = 0x0000;
	_addr_abs = 0x0000;
	_fetched = 0x00;

	//resets take times so We are gonna internally hardcode it
	_cycles = 8;
}

/// @brief interrupt request
void CPU_6502::interupt_requested()
{
	// It can be ignore if the diable Interrupt bit has npt been set
	// We want to inteerupt the programm
	// So we will save the current status of the programm in the stack
	if (GetFlag(I) == 0) {
		write(0x100 + _stack_pointer, (_program_counter >> 8) & 0x00FF );
		_stack_pointer--;
		write(0x0100 + _stack_pointer, _program_counter & 0x00FF);
		_stack_pointer--;


		//Some bits has been said to indicated an interruption has occurred
		Setflag(B, 0);
		Setflag(U, 1);
		Setflag(I, 1);
		// Then Push the status register to the stack
		write(0x0100 + _stack_pointer, _status_register);
		_stack_pointer--;

		// Read new program counter location from fixed address

		// It then force the program to jump into a location set by the programmer if an intteruption occurs
		_addr_abs = 0xFFFE;
		uint16_t lo = read(_addr_abs + 0);
		uint16_t hi = read(_addr_abs + 1);
		_program_counter = (hi << 8) | lo;


		// IRQs take time
		_cycles = 7;

	}

}

/// @brief interrupt request non masquable. Same as IRQ BUTTTT we jump program counter into another address
void CPU_6502::non_masquable_interupt_request()
{
	// We want to inteerupt the programm
	// So we will save the current status of the programm in the stack
	write(0x100 + _stack_pointer, (_program_counter >> 8) & 0x00FF );
	_stack_pointer--;
	write(0x0100 + _stack_pointer, _program_counter & 0x00FF);
	_stack_pointer--;
	//Some bits has been said to indicated an interruption has occurred
	Setflag(B, 0);
	Setflag(U, 1);
	Setflag(I, 1);
	// Then Push the status register to the stack
	write(0x0100 + _stack_pointer, _status_register);
	_stack_pointer--;
	// Read new program counter location from fixed address
	// It then force the program to jump into a location set by the programmer if an intteruption occurs


	_addr_abs = 0xFFFA;
	uint16_t lo = read(_addr_abs + 0);
	uint16_t hi = read(_addr_abs + 1);
	_program_counter = (hi << 8) | lo;
	// IRQs take time
	_cycles = 7;
}


/// @brief Return From interupt. It restore the state of the CPU Before the interruption
uint8_t CPU_6502::RTI()
{
	// we read the status register from the stack
	_stack_pointer++;
	_status_register = read(0x100 + _stack_pointer);
	// We unset interuption bits
	_status_register &= ~B;
	_status_register &= ~U;

	// We fill the program counter with its original value

	_stack_pointer++;
	_program_counter = (uint16_t)read(0x0100 + _stack_pointer);
	_stack_pointer++;
	_program_counter |= read(0x0100 + _stack_pointer) << 8;
	return 0;

}

bool CPU_6502::complete()
{
	return _cycles == 0;
}

// This is the disassembly function. Its workings are not required for emulation.
// It is merely a convenience function to turn the binary instruction code into
// human readable form. Its included as part of the emulator because it can take
// advantage of many of the CPUs internal operations to do this.
std::map<uint16_t, std::string> CPU_6502::disassemble(uint16_t nStart, uint16_t nStop)
{
	uint32_t addr = nStart;
	uint8_t value = 0x00, lo = 0x00, hi = 0x00;
	std::map<uint16_t, std::string> mapLines;
	uint16_t line_addr = 0;

	// A convenient utility to convert variables into
	// hex strings because "modern C++"'s method with 
	// streams is atrocious
	auto hex = [](uint32_t n, uint8_t d)
	{
		std::string s(d, '0');
		for (int i = d - 1; i >= 0; i--, n >>= 4)
			s[i] = "0123456789ABCDEF"[n & 0xF];
		return s;
	};

	// Starting at the specified address we read an instruction
	// byte, which in turn yields information from the lookup table
	// as to how many additional bytes we need to read and what the
	// addressing mode is. I need this info to assemble human readable
	// syntax, which is different depending upon the addressing mode

	// As the instruction is decoded, a std::string is assembled
	// with the readable output
	while (addr <= (uint32_t)nStop)
	{
		line_addr = addr;

		// Prefix line with instruction address
		std::string sInst = "$" + hex(addr, 4) + ": ";

		// Read instruction, and get its readable name
		uint8_t opcode = _bus->read(addr, true); addr++;
		sInst += _lookup[opcode].name + " ";

		// Get oprands from desired locations, and form the
		// instruction based upon its addressing mode. These
		// routines mimmick the actual fetch routine of the
		// 6502 in order to get accurate data as part of the
		// instruction
		if (_lookup[opcode].addrmode == &CPU_6502::IMP)
		{
			sInst += " {IMP}";
		}
		else if (_lookup[opcode].addrmode == &CPU_6502::IMM)
		{
			value = _bus->read(addr, true); addr++;
			sInst += "#$" + hex(value, 2) + " {IMM}";
		}
		else if (_lookup[opcode].addrmode == &CPU_6502::ZP0)
		{
			lo = _bus->read(addr, true); addr++;
			hi = 0x00;												
			sInst += "$" + hex(lo, 2) + " {ZP0}";
		}
		else if (_lookup[opcode].addrmode == &CPU_6502::ZPX)
		{
			lo = _bus->read(addr, true); addr++;
			hi = 0x00;														
			sInst += "$" + hex(lo, 2) + ", X {ZPX}";
		}
		else if (_lookup[opcode].addrmode == &CPU_6502::ZPY)
		{
			lo = _bus->read(addr, true); addr++;
			hi = 0x00;														
			sInst += "$" + hex(lo, 2) + ", Y {ZPY}";
		}
		else if (_lookup[opcode].addrmode == &CPU_6502::IZX)
		{
			lo = _bus->read(addr, true); addr++;
			hi = 0x00;								
			sInst += "($" + hex(lo, 2) + ", X) {IZX}";
		}
		else if (_lookup[opcode].addrmode == &CPU_6502::IZY)
		{
			lo = _bus->read(addr, true); addr++;
			hi = 0x00;								
			sInst += "($" + hex(lo, 2) + "), Y {IZY}";
		}
		else if (_lookup[opcode].addrmode == &CPU_6502::ABS)
		{
			lo = _bus->read(addr, true); addr++;
			hi = _bus->read(addr, true); addr++;
			sInst += "$" + hex((uint16_t)(hi << 8) | lo, 4) + " {ABS}";
		}
		else if (_lookup[opcode].addrmode == &CPU_6502::ABX)
		{
			lo = _bus->read(addr, true); addr++;
			hi = _bus->read(addr, true); addr++;
			sInst += "$" + hex((uint16_t)(hi << 8) | lo, 4) + ", X {ABX}";
		}
		else if (_lookup[opcode].addrmode == &CPU_6502::ABY)
		{
			lo = _bus->read(addr, true); addr++;
			hi = _bus->read(addr, true); addr++;
			sInst += "$" + hex((uint16_t)(hi << 8) | lo, 4) + ", Y {ABY}";
		}
		else if (_lookup[opcode].addrmode == &CPU_6502::IND)
		{
			lo = _bus->read(addr, true); addr++;
			hi = _bus->read(addr, true); addr++;
			sInst += "($" + hex((uint16_t)(hi << 8) | lo, 4) + ") {IND}";
		}
		else if (_lookup[opcode].addrmode == &CPU_6502::REL)
		{
			value = _bus->read(addr, true); addr++;
			sInst += "$" + hex(value, 2) + " [$" + hex(addr + value, 4) + "] {REL}";
		}

		// Add the formed string to a std::map, using the instruction's
		// address as the key. This makes it convenient to look for later
		// as the instructions are variable in length, so a straight up
		// incremental index is not sufficient.
		mapLines[line_addr] = sInst;
	}

	return mapLines;
}
