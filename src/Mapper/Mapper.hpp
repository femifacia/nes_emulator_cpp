#pragma once
#include <iostream>
#include <cstdint>

class Mapper
{
private:
    /* data */

protected:
    uint8_t _prgBanks;
    uint8_t _chrBanks;
public:
    Mapper(uint8_t prgBanks, uint8_t chrBanks);
    ~Mapper();
    virtual bool cpuMapRead(uint16_t addr, uint32_t &mapped_addr)	 = 0;
	virtual bool cpuMapWrite(uint16_t addr, uint32_t &mapped_addr)	 = 0;
	// Transform PPU bus address into CHR ROM offset
	virtual bool ppuMapRead(uint16_t addr, uint32_t &mapped_addr)	 = 0;
	virtual bool ppuMapWrite(uint16_t addr, uint32_t &mapped_addr)	 = 0;
};

