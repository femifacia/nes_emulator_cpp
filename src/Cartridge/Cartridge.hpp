#pragma once
#include <cstdint>
#include <vector>
#include <iostream>
#include <fstream>
#include <memory>
#include "../Mapper/Mapper_000/Mapper_000.hpp"

class Cartridge
{
private:
    /* data */


    // a representation of the memory for the PRG and the CHR section
    std::vector<uint8_t> _PRGMemory;
    std::vector<uint8_t> _CHRMemory;
    

    // the mapper we are currently using
    uint8_t _nMapperID;

    // how many banks of PRG and CHR memory there are
    uint8_t _nCHRBanks;
    uint8_t _nPRGBanks;
    std::shared_ptr<Mapper> _mapperPtr;

public:
    Cartridge(const std::string &pathToFile);
    ~Cartridge();

    //here the read and write functions are returning a bool indicating wether or not the cartridge is handling that read or write


    // communication with the CPU BUS
    bool cpuRead(uint16_t addr, uint8_t &data);
    bool cpuWrite(uint16_t addr, uint8_t data);



    // Communication with the PPU and its components
    bool ppuRead(uint16_t addr, uint8_t &data);
    bool ppuWrite(uint16_t addr, uint8_t data);
};


