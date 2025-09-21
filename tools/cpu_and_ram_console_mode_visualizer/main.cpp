#include "../../src/Bus/Bus.hpp"

void drawNes(Bus *nes)
{
    std::cout << "NES" << std::endl;
}

int main(int argc, const char** argv)
{
    Bus nes;
//    std::string input("");
    char input = 0;

    while (1) {
        input = std::getchar();
        std::cout  << input <<std::endl;
        if (input == 'q')
            break;
        if (input == 'r') {
            nes._cpu.reset();
        } else {
            nes._cpu.clock();
        }
        drawNes(&nes);
    }
    std::cout << "SaA" << std::endl;
    return 0;
}