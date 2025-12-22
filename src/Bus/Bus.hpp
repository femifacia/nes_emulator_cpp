#pragma once
#include <iostream>
#include <cstdint> // c standard integer Lib rename the standard types into explicit types
#include <array>
#include "../CPU_6502/CPU_6502.hpp"
#include <memory>
#include <string>
#include <cstring>
#include "../PPU_2C02/PPU_2C02.hpp"

class Bus
{
private: 



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
};