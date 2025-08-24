#include "Bus.hpp"


Bus::Bus(/* args */)
{
    for (auto &i : _ram)
        i = 0;
}

Bus::~Bus()
{
}
