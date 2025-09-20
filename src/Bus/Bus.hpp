#pragma once
#include <iostream>
#include <cstdint> // c standard integer Lib rename the standard types into explicit types
#include <array>
#include "../CPU_6502/CPU_6502.hpp"
#include <memory>
#include <string>
#include <cstring>

class Bus
{
private: 



public:
    Bus(/* args */);
    ~Bus();

        /* Devices on the BUS */
    CPU_6502 _cpu;


    /* RAM*/
    std::array<uint8_t, 64 * 1024> _ram; // 64KB ram

    /// @brief Write Data on the bus
    /// @param addr 
    /// @param data 
    void write(uint16_t addr, uint8_t data);
    


    /// @brief Read data from the bus
    /// @param addr 
    /// @param bReadOnly 
    /// @return 
    uint8_t read(uint16_t addr, bool bReadOnly = false);
};