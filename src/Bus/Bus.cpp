#include "Bus.hpp"


Bus::Bus(/* args */)
{
    std::memset(_ram.data(), 0, _ram.size() * sizeof(decltype(_ram)::value_type));
//    for (auto &i : _ram)
//        i = 0;

    _cpu.ConnectBus(this);
}

Bus::~Bus()
{
}



void Bus::write(uint16_t addr, uint8_t data)
{
    //Because Here the only device On the Bus is The RAM, every writing request will be direcetly adressed to the ram
    // If there was another device, we should have use a mapper to knwow which devices respond to an adress in the range inputed and give it

    // The ram only store the information at the adresse inputed
    // The ram take the full BUS so the requested addr had to be positive and lower than 0XFFFF

    if (addr >= 0X0000 && addr <=  0xFFFF) {
        _ram[addr] = data;
    }
}


uint8_t Bus::read(uint16_t addr, bool bReadOnly)
{

    if (addr >= 0X0000 && addr <=  0xFFFF) {
        return _ram[addr];
    }
    return 0x00;

}