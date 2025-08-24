#pragma once
#include <iostream>
#include <cstdint> // c standard integer Lib rename the standard types into explicit types
#include <array>
#include "../OLC_6502/OLC_6502.hpp"
#include <memory>
#include <string>

class Bus
{
private: 
    /* Devices on the BUS */
    OLC_6502 _cpu;

    /* RAM*/
    std::array<uint8_t, 64 * 1024> _ram; // 64KB ram

public:
    Bus(/* args */);
    ~Bus();

    /// @brief Write Data on the bus
    /// @param addr 
    /// @param data 
    void write(uint16_t addr, uint8_t data);
    


    uint8_t read(uint16_t addr, bool bReadOnly = false);
};