#include <cstdint>
#include <iostream>
#include <vector>
#include <map>

class PPU_2C02
{
private:
    /* data */
public:
    PPU_2C02(/* args */);
    ~PPU_2C02();


    // communication with the CPU
    uint8_t cpuRead(uint16_t addr, bool readOnly=false);
    void cpuWrite(uint16_t addr, uint8_t data);



    // Communication with the PPU and its components
    uint8_t ppuRead(uint16_t addr, bool readOnly=false);
    void ppuWrite(uint16_t addr, uint8_t data);
};
