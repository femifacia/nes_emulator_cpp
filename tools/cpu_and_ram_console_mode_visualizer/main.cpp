#include "../../src/Bus/Bus.hpp"
#include <sstream>

std::string hex(uint32_t n, uint8_t d)
{
	std::string s(d, '0');
	for (int i = d - 1; i >= 0; i--, n >>= 4)
		s[i] = "0123456789ABCDEF"[n & 0xF];
	return s;
};

void drawRamRange(Bus *nes, uint16_t offset, uint8_t lines, uint8_t col, std::string color = "\033[01;32m")
{
    std::string stop_color("\033[00m");
    std::cout << color << "RAM\033[00m Range From " << color <<"$"  << hex(offset, 4)<< stop_color << " To ";
    std::cout << color << "$"<< hex(offset + ((lines - 1) * 16) + col - 1, 4) << stop_color<< std::endl << std::endl;
    for (int i = 0; i < lines; i++) {
        std::cout << color << "$" << hex(offset + (i * 16), 4) << " \033[00m";
        for (int j = 0; j < col; j++) {
            if (nes->_ram[offset + (i * 16) + j])
                std::cout << color;
            std::cout << hex(nes->_ram[offset + (i * 16) + j], 2);
            if (j + 1 < col)
                std::cout << stop_color << " ";
        }
        std::cout  << std::endl;
    }
    std::cout << std::endl;
}


void drawCpuStatus(Bus *nes)
{
    std::string blue("\033[34m");
    std::string red("\033[31m");
    std::string green("\033[32m");
    std::string stop_color("\033[00m");
    std::string cyan("\033[36m");

    std::cout << "----CPU status------" << std::endl;
    // status register
    std::cout << blue << "[Status Register]: ";
    std::cout << (((nes->_cpu._status_register & (CPU_6502::FLAGS6502::C) ) > 0) ? green : red) << "C" << stop_color << " "; 
    std::cout << (((nes->_cpu._status_register & (CPU_6502::FLAGS6502::Z) ) > 0) ? green : red) << "Z" << stop_color << " "; 
    std::cout << (((nes->_cpu._status_register & (CPU_6502::FLAGS6502::I) ) > 0) ? green : red) << "I" << stop_color << " "; 
    std::cout << (((nes->_cpu._status_register & (CPU_6502::FLAGS6502::D) ) > 0) ? green : red) << "D" << stop_color << " "; 
    std::cout << (((nes->_cpu._status_register & (CPU_6502::FLAGS6502::B) ) > 0) ? green : red) << "B" << stop_color << " "; 
    std::cout << (((nes->_cpu._status_register & (CPU_6502::FLAGS6502::U) ) > 0) ? green : red) << "U" << stop_color << " "; 
    std::cout << (((nes->_cpu._status_register & (CPU_6502::FLAGS6502::V) ) > 0) ? green : red) << "V" << stop_color << " "; 
    std::cout << (((nes->_cpu._status_register & (CPU_6502::FLAGS6502::N) ) > 0) ? green : red) << "N" << stop_color << " ";
    std::cout << std::endl;

    // other registers
    std::cout << blue << "[X Register]: " << (nes->_cpu._x ? cyan : stop_color )<<"$" << hex(nes->_cpu._x, 2) << stop_color<< std::endl;
    std::cout << blue << "[Y Register]: " << (nes->_cpu._y ? cyan : stop_color ) << "$" <<hex(nes->_cpu._y, 2) << stop_color<< std::endl;
    std::cout << blue << "[Accumulator Register]: " << (nes->_cpu._accumulator_register ? cyan : stop_color ) << "$"<< hex(nes->_cpu._accumulator_register, 2) << stop_color<< std::endl;
    std::cout << blue << "[Stack Pointer]: " << (nes->_cpu._stack_pointer ? cyan : stop_color ) << "$"<< hex(nes->_cpu._stack_pointer, 2) << stop_color<< std::endl;

    
}

void drawCurrentStatus(Bus *nes, uint16_t start, uint16_t end)
{
    std::string stop_color("\033[00m");
    std::string cyan("\033[36m");
    std::string violet("\033[35m");
    std::cout << std::endl << violet << "CURRENT STATUS" << stop_color<< std::endl;
    auto disassembly = nes->_cpu.disassemble(start, end);

    for (const auto& [addr, instr] : disassembly) {
        if (addr == nes->_cpu._program_counter)
            std::cout << cyan;
       std::cout << instr << stop_color << "\n";
    }
}

void drawNes(Bus *nes, uint16_t offset)
{
    std::cout << "NES"<< std::endl;
    // Draw RAM Pages
    drawRamRange(nes, 0x0000, 16, 16);
    drawRamRange(nes, offset, 16, 16, "\033[01;31m");
    // Draw CPU Status
    drawCpuStatus(nes);
    // Draw Current Status
    drawCurrentStatus(nes, nes->_cpu._program_counter-10, nes->_cpu._program_counter + 10);
    
}

int main(int argc, const char** argv)
{
    Bus nes;
//    std::string input("");
    char input = 0;
    uint16_t offset = 0x6000;

    std::stringstream program_object_code;

    program_object_code << "A2 0A 8E 00 00 A2 03 8E 01 00 AC 00 00 A9 00 18 6D 01 00 88 D0 FA 8D 02 00 EA EA EA";
	uint16_t nOffset = offset;
	while (!program_object_code.eof())
	{
		std::string b;
		program_object_code >> b;
		nes._ram[nOffset++] = (uint8_t)std::stoul(b, nullptr, 16);
	}

    // The program counter will start at offset. So we have to indicate in the ram at 0xFFFE and 0xFFFF how offset splited
    // Because when a BRK happens, the program counter is reset to the value located at these places
    nes._cpu._program_counter = offset;

    // low byte
    nes._ram[0xFFFE] = (offset  & 0xFF);
    // high byte
    nes._ram[0xFFFF] = offset >> 8;

    // we do the same for reset
        // low byte
    nes._ram[0xFFFC] = (offset  & 0xFF);
    // high byte
    nes._ram[0xFFFD] = offset >> 8;

    while (1) {
        drawNes(&nes, offset);
        input = std::getchar();
        std::cout  << input <<std::endl;
        if (input == 'q')
            break;
        if (input == 'r') {
            nes._cpu.reset();
        } else {
            nes._cpu.clock();
            while (!nes._cpu.complete())
                nes._cpu.clock();
        }
 //       nes._ram[10] += 1;
//        nes._ram[0X6020] += 1;
    }
    std::cout << "SaA" << std::endl;
    return 0;
}