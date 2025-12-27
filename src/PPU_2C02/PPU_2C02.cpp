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

    switch (addr)
    {
    case CONTROL:
        /* code */
        break;
    case MASK:
        break;
    case STATUS:
        break;
    case OAM_ADDR:
        break;
    case OAM_DATA:
        break;    
    case SCROLL:
        break;
    case PPU_ADDR:
        break;
    case PPU_DATA:
        break;
    }

    return data;

}
void PPU_2C02::cpuWrite(uint16_t addr, uint8_t data)
{
    switch (addr)
    {
    case CONTROL:
        /* code */
        break;
    case MASK:
        break;
    case STATUS:
        break;
    case OAM_ADDR:
        break;
    case OAM_DATA:
        break;    
    case SCROLL:
        break;
    case PPU_ADDR:
        break;
    case PPU_DATA:
        break;
    }

}
// Communication with the PPU and its components
uint8_t PPU_2C02::ppuRead(uint16_t addr, bool readOnly=false)
{
    uint8_t data = 0x00;

    addr &= 0x3FFF; // the ppu has a VRAM of 16KB on its own bus. So its gos from 0 to 0x3FFF then mirroring
    return data;
}


void  PPU_2C02::ppuWrite(uint16_t addr, uint8_t data)
{
    addr &= 0x3FFF; // the ppu has a VRAM of 16KB on its own bus. So its gos from 0 to 0x3FFF then mirroring

}


void PPU_2C02::ConnectCartridge(const std::shared_ptr<Cartridge> &cartridge)
{
    _cartridge = cartridge;
}

void PPU_2C02::clock()
{
}