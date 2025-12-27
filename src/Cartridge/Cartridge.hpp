#pragma once
#include <cstdint>

class Cartridge
{
private:
    /* data */
public:
    Cartridge(/* args */);
    ~Cartridge();


    // communication with the CPU BUS
    uint8_t cpuRead(uint16_t addr, bool readOnly=false);
    void cpuWrite(uint16_t addr, uint8_t data);



    // Communication with the PPU and its components
    uint8_t ppuRead(uint16_t addr, bool readOnly=false);
    void ppuWrite(uint16_t addr, uint8_t data);
};


