#include "../../src/Bus/Bus.hpp"

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
    std::string cyan("\033[35m");

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

    
}

void drawNes(Bus *nes, uint16_t offset)
{
    std::cout << "NES"<< std::endl;
    // Draw RAM Pages
    drawRamRange(nes, 0x0000, 16, 16);
    drawRamRange(nes, offset, 16, 16, "\033[01;31m");
    // Draw CPU Status
    drawCpuStatus(nes);
    
}

int main(int argc, const char** argv)
{
    Bus nes;
//    std::string input("");
    char input = 0;
    uint16_t offset = 0x6000;

    while (1) {
        input = std::getchar();
        std::cout  << input <<std::endl;
        if (input == 'q')
            break;
        if (input == 'r') {
            nes._cpu.reset();
        } else {
            nes._cpu.clock();
        }
        drawNes(&nes, offset);
        nes._ram[10] += 1;
        nes._ram[0X6020] += 1;
    }
    std::cout << "SaA" << std::endl;
    return 0;
}