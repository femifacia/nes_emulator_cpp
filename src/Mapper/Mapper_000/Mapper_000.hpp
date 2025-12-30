#pragma once
#include "../Mapper.hpp"


class Mapper_000 : public Mapper
{
private:
    /* data */
public:
    Mapper_000(uint8_t prgBanks, uint8_t chrBanks);
    ~Mapper_000();

    bool cpuMapRead(uint16_t addr, uint32_t &mapped_addr) override;	
    bool cpuMapWrite(uint16_t addr, uint32_t &mapped_addr) override;	
	// Transform PPU bus address into CHR ROM offset
    bool ppuMapRead(uint16_t addr, uint32_t &mapped_addr) override;	
    bool ppuMapWrite(uint16_t addr, uint32_t &mapped_addr) override;	
};

