#include "OLC_6502.hpp"
#include "../Bus/Bus.hpp"

OLC_6502::OLC_6502(/* args */)
{
    _bus = nullptr;
}

OLC_6502::~OLC_6502()
{
}


void OLC_6502::ConnectBus(Bus *bus)
{
    _bus = bus;
}

void OLC_6502::write(uint16_t addr, uint8_t data)
{
    _bus->write(addr, data);
}

uint8_t OLC_6502::read(uint16_t addr)
{
    return _bus->read(addr);
}