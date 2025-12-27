#include "PPU_2C02.hpp"

PPU_2C02::PPU_2C02(/* args */)
{
}

PPU_2C02::~PPU_2C02()
{
}

uint8_t PPU_2C02::cpuRead(uint16_t addr, bool readOnly=false)
{
    uint8_t data = 0x00;

    return data;

}
void PPU_2C02::cpuWrite(uint16_t addr, uint8_t data)
{

}
// Communication with the PPU and its components
uint8_t ppuRead(uint16_t addr, bool readOnly=false)
{

}


void  PPU_2C02::ppuWrite(uint16_t addr, uint8_t data)
{

}
