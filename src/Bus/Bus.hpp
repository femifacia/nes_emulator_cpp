#pragma once
#include <iostream>
#include <cstdint> // c standard integer Lib rename the standard types into explicit types
#include <array>
#include "../CPU_6502/CPU_6502.hpp"
#include <memory>
#include <string>
#include <cstring>
#include "../PPU_2C02/PPU_2C02.hpp"
#include "../Cartridge/Cartridge.hpp"

class Bus
{
private: 

    uint32_t _numberSystemClock;
    std::shared_ptr<Cartridge> _cartridge;



public:
    Bus(/* args */);
    ~Bus();

        /* Devices on the BUS */
    CPU_6502 _cpu;


    /* RAM*/
    std::array<uint8_t, 2048> _cpuRam; // 64KB ram


    // the ppu is connected to the cpu bus
    PPU_2C02 _ppu;

    /// @brief Write Data on the bus
    /// @param addr 
    /// @param data 
    void cpuWrite(uint16_t addr, uint8_t data);
    


    /// @brief Read data from the bus
    /// @param addr 
    /// @param bReadOnly 
    /// @return 
    uint8_t cpuRead(uint16_t addr, bool bReadOnly = false);


    /// @brief Load a cartridge into the nes before launching it
    /// @param cartridge the cartridge to load
    void insertCartridge(const std::shared_ptr<Cartridge> &cartridge); // here we load the cartridge on the nes before launching the game

    /// @brief reset the internal system
    void reset(); // the reset button of the nes

    /// @brief count a clock
    void clock(); // clock system


};