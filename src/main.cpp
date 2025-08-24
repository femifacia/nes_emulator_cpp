#include <iostream>
#include "./Bus/Bus.hpp"

int main(int argc, char **argv)
{
    Bus bus;

    bus.write(5, 90);
    uint8_t val = bus.read(5);
    std::cout << val << std::endl;
    return 0;
}