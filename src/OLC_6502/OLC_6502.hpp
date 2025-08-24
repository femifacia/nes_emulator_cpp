#pragma once
#include <cstdint>
#include <iostream>
class Bus;

class OLC_6502
{
private:
    /* data */

    Bus *_bus;
public:
    OLC_6502(/* args */);
    ~OLC_6502();

    /// @brief connect to the BUS
    /// @param bus 
    void ConnectBus(Bus *bus);


    /// @brief write data on the bus
    /// @param addr 
    /// @param data 
    void write(uint16_t addr, uint8_t data);

    /// @brief read data from the bus
    /// @param addr 
    /// @return 
    uint8_t read(uint16_t addr);
};

