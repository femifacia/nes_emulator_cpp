#include "Cartridge.hpp"


Cartridge::Cartridge(const std::string& path):_nMapperID(0),_nCHRBanks(0),_nPRGBanks(0)
{

    // it is a iNes file format header

    // look at the doc to understand the fileds
    // The 04 first bytes are N E S and MS-DOM
    struct inesHeader_t
    {
        char name[4];
        uint8_t prg_rom_chunks;
        uint8_t chr_rom_chunks;
        uint8_t mapper1;
        uint8_t mapper2;
        uint8_t prg_ram_size;
        uint8_t tv_system1;
        uint8_t tv_system2;
        char unusued[5];
    } header;

    std::ifstream ifs;
    ifs.open(path, std::ifstream::binary);

    if (!ifs.is_open()) {
        std::cout << "Cannot open the cartridge at " << path << std::endl;
        return;
    }
    ifs.read((char *)&header, sizeof(inesHeader_t));

    //ifs.read(reinterpret_cast<char*>(&header), sizeof(inesHeader_t));


    // Example logic for retrieving the Mapper ID (often split across 2 bytes in iNES)
    // The 4 most significant bits of mapper1 are the least significant bits of the MapperID
    // The 4 most significant bits of mapper2 are the most significant bits of the MapperID
    // It id detailled on the documentation
    _nMapperID = ((header.mapper2 >> 4) << 4) | (header.mapper1 >> 4);


    // The documentation tells us the next 512 bytes are used for training informations.
    // So we have to ignore them
    if (header.mapper1 & 0x04) {
        ifs.seekg(512, std::ios_base::cur);
    }

    // We have 03 Types of iNes Files

    uint8_t fileType = 1;

    if (fileType == 0) {

    }


    if (fileType == 1) {

        // We are reading how many banks memory are for the PRG and the CHR and we will resize ours vectors and read those memories
        // a single bank of PRG memory is 16kb and a single bank of CHR memory is 8kb

        _nPRGBanks = header.prg_rom_chunks;
        _PRGMemory.resize(_nPRGBanks * 16384);
        ifs.read((char *)_PRGMemory.data(), _PRGMemory.size());

        _nCHRBanks = header.chr_rom_chunks;
        _CHRMemory.resize(_nCHRBanks * 8192);
        ifs.read((char *)_CHRMemory.data(), _CHRMemory.size());
        
    }


    if (fileType == 2) {
        
    }

    // load the appropriate mapper
    switch (_nMapperID)
    {
    case 0:
        /* code */
        _mapperPtr = std::make_shared<Mapper_000>(_nPRGBanks, _nCHRBanks);
        break;
    
    }

    ifs.close();

}

Cartridge::~Cartridge()
{
}

bool Cartridge::cpuRead(uint16_t addr, uint8_t &data)
{
    // on our implementation, cartridge has a veto right on the memorry transaction
    // if a read is intercepted, we have to check if this read is destined to the cartridge.
    // The only component which has the ability to clear this interrogation is the Mapper.

    // Through its cpuMapRead function, we can know if the read addr is destined to the cartridge or not
    uint32_t mapped_addr = 0;
    if (!(_mapperPtr->cpuMapRead(addr, mapped_addr)))
        return false;
    // on this branch of code, we know the read is destined to the cartridge,
    // the mapped_addr has been passed in reference to the cpuMapRead and know contain the address of the data mapped

    // and remeber !, the cpu interact with the PRG memory

    data = _PRGMemory[mapped_addr];

    // We return true to indicates the cartridge handles this particular addr
    return true;
}

bool Cartridge::cpuWrite(uint16_t addr, uint8_t data)
{
    // this is the same logic than the read case
    uint32_t mapped_addr = 0;
    if (!(_mapperPtr->cpuMapWrite(addr, mapped_addr)))
        return false;

    _PRGMemory[mapped_addr] = data;
    return true;
}

bool Cartridge::ppuRead(uint16_t addr,uint8_t &data)
{
    // we will apply the same logic with the only difference being we will interact with the CHR memory (ppu interact with CHR)
    uint32_t mapped_addr = 0;
    if (!(_mapperPtr->ppuMapRead(addr, mapped_addr)))
        return false;

    data = _CHRMemory[mapped_addr];

    // We return true to indicates the cartridge handles this particular addr
    return true;
}

bool Cartridge::ppuWrite(uint16_t addr, uint8_t data)
{   uint32_t mapped_addr = 0;
//    if (!(_mapperPtr->ppuMapWrite(addr, mapped_addr)))
    if (!(_mapperPtr->ppuMapRead(addr, mapped_addr)))
        return false;

    _CHRMemory[mapped_addr] = data;
    return true;
}