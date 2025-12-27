#include <cstdint>
#include <iostream>
#include <vector>
#include <memory>
#include <map>
#include "../Cartridge/Cartridge.hpp"

class PPU_2C02
{
private:
    // The PPU also have access to the cartridge

    /// @brief the cartridge or Gamepak
    std::shared_ptr<Cartridge> _cartridge;

    // here we will materialize the VRAM carried by the PPU

    // Each of the following elements are connected on the PPU BUS

    uint8_t _nameTable[2][1024];
    uint8_t _paletteTable[32];
    uint8_t _patternTable[2][4096]; // Normally, the pattern table is in the cartridge but Javid want to reimplement it here for fututre improvements
    

    /* data */
public:
    PPU_2C02(/* args */);
    ~PPU_2C02();

    enum PPU_REGISTERS
    {
        CONTROL = 0x0000,
        MASK = 0x0001,
        STATUS = 0x0002,
        OAM_ADDR = 0x0003,
        OAM_DATA = 0x0004,
        SCROLL = 0x0005,
        PPU_ADDR = 0x0006,
        PPU_DATA = 0x0007

    };


    // communication with the CPU
    uint8_t cpuRead(uint16_t addr, bool readOnly=false);
    void cpuWrite(uint16_t addr, uint8_t data);



    // Communication with the PPU and its components
    uint8_t ppuRead(uint16_t addr, bool readOnly=false);
    void ppuWrite(uint16_t addr, uint8_t data);

    void ConnectCartridge(const std::shared_ptr<Cartridge> &cart);
    void clock();
};
