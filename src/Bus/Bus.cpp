#include "Bus.hpp"


Bus::Bus(/* args */):_numberSystemClock(0)
{
    std::memset(_cpuRam.data(), 0, _cpuRam.size() * sizeof(decltype(_cpuRam)::value_type));
//    for (auto &i : _ram)
//        i = 0;

    _cpu.ConnectBus(this);
}

Bus::~Bus()
{
}



void Bus::cpuWrite(uint16_t addr, uint8_t data)
{
    //Because Here the only device On the Bus is The RAM, every writing request will be direcetly adressed to the ram
    // If there was another device, we should have use a mapper to knwow which devices respond to an adress in the range inputed and give it

    // The ram only store the information at the adresse inputed
    // The ram take the full BUS so the requested addr had to be positive and lower than 0XFFFF


    // the cartridge having the premium on the main bus is something Javid wanted to add for further improvements

    if (_cartridge->cpuWrite(addr, data)) {

    } else if (addr >= 0X0000 && addr <=  0x1FFF) {
        // we are applying mirroring principle
        _cpuRam[addr & 0x07FF] = data;
//        _cpuRam[addr % 2048] = data;
    } else if (addr >= 0x2000 && addr <= 0x3FFF) {
        _ppu.cpuWrite(addr & 0x0007, data);
    }
}


uint8_t Bus::cpuRead(uint16_t addr, bool bReadOnly)
{
    uint8_t data = 0x00;

    // the cartridge having the premium on the main bus is something Javid wanted to add for further improvements

    if (_cartridge->cpuRead(addr, data)) {

    } else if (addr >= 0X0000 && addr <=  0x1FFF) {
        // we are applying mirroring principle
        data = _cpuRam[addr & 0x07FF];
//        data = _cpuRam[addr % 2048];
    } else if (addr >= 0x2000 && addr <= 0x3FFF) {
        data = _ppu.cpuRead(addr & 0x0007, bReadOnly);
    }
    return data;

}


void Bus::reset()
{
    _cpu.reset();
    _numberSystemClock = 0;
}

void Bus::clock()
{

}

void Bus::insertCartridge(const std::shared_ptr<Cartridge> &cartridge)
{
    this->_cartridge = cartridge;
    _ppu.ConnectCartridge(cartridge);
}