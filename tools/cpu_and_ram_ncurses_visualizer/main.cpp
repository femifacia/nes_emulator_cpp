#include <iostream>
#include <ncurses.h>
#include "../../src/Bus/Bus.hpp"
#include <sstream>
#include <array>
#include <cstdint>
#include <algorithm>


void ncurseDemo(void)
{
    
    attron(A_NORMAL);
    printw("Texte sans mise en forme\n");

    attron(A_STANDOUT);
    printw("Texte en meilleur surlignement\n");
    attroff(A_STANDOUT);

    attron(A_REVERSE);
    printw("Texte en inversion video\n");
    attroff(A_REVERSE);

    attron(A_DIM);
    printw("Texte a moitie brillant\n");
    attroff(A_DIM);

    attron(A_BOLD);
    printw("Texte en gras\n");
    attroff(A_BOLD);

    attron(A_UNDERLINE);
    printw("Texte en souligne\n");
    attroff(A_UNDERLINE);

    attron(A_INVIS);
    printw("Texte invisible\n");
    attroff(A_INVIS);

    attron(A_UNDERLINE | A_BOLD); // Pour appliquer plusieurs type de mises en forme, on utilise l'opérateur unaire |
    printw("Texte en gras souligne\n");

    init_pair(1, COLOR_RED, COLOR_BLUE);

    attroff(A_UNDERLINE | A_BOLD);
    attron(A_NORMAL);
    printw("heheheh \n");
    attron(COLOR_PAIR(1));
    printw("heheheh\n");
    refresh();
    getch();
    endwin();
}

void initNcursesColors()
{
    init_pair(1, COLOR_BLUE, COLOR_BLACK);
    init_pair(3, COLOR_GREEN, COLOR_BLACK);
    init_pair(2, COLOR_YELLOW, COLOR_BLACK);
    init_pair(1, COLOR_BLUE, COLOR_BLACK);
    init_pair(1, COLOR_BLUE, COLOR_BLACK);
    init_pair(7, COLOR_YELLOW, COLOR_WHITE);
    init_pair(8, COLOR_GREEN, COLOR_BLACK);
    init_pair(9, COLOR_RED, COLOR_BLACK);
    init_pair(10, COLOR_WHITE, COLOR_GREEN);
    init_pair(11, COLOR_WHITE, COLOR_BLUE);
}


std::string hex(uint32_t n, uint8_t d)
{
	std::string s(d, '0');
	for (int i = d - 1; i >= 0; i--, n >>= 4)
		s[i] = "0123456789ABCDEF"[n & 0xF];
	return s;
};

void drawRamRange(Bus *nes, uint16_t offset, uint8_t lines, uint8_t col, int x, int y, uint8_t color_title, uint8_t color_important_info = 1,uint8_t color_text=3)
{
    std::string stop_color("\033[00m");
    move(y,x);

    attron(COLOR_PAIR(color_title));
    attron(A_BOLD);
    addstr("RAM: [");
    attron(COLOR_PAIR(color_important_info));
    addstr(hex(offset, 4).c_str());
    addstr(" : ");
    attron(COLOR_PAIR(color_title));
    attron(COLOR_PAIR(color_important_info));
    addstr( hex(offset + ((lines - 1) * 16) + col - 1, 4).c_str());
    attron(COLOR_PAIR(color_title));
    addstr("]");
    addstr("");
    addstr("\n");
    attroff(COLOR_PAIR(color_title));

//    attroff(A_BOLD);
//    attroff(COLOR_PAIR(color_title));
//    addstr("RAM");

    
    for (int i = 0; i < lines; i++) {
        attroff(A_COLOR);
        addstr("$");
        addstr(hex(offset + (i * 16), 4).c_str());
        addstr(" :");

        //std::cout << color << "$" << hex(offset + (i * 16), 4) << " \033[00m";
        for (int j = 0; j < col; j++) {
            attroff(COLOR_PAIR(color_important_info));
            if (nes->_ram[offset + (i * 16) + j])
                attron(COLOR_PAIR(color_important_info));
            else
                attron(COLOR_PAIR(color_text));

                
            addstr(hex(nes->_ram[offset + (i * 16) + j], 2).c_str());
            if (j + 1 < col)
                addstr(" ");
        }
        addstr("\n");

    }
    addstr("\n");

}

void rectansgle(int y1, int x1, int y2, int x2)
{
    mvhline(y1, x1, 0, x2-x1);
    mvhline(y2, x1, 0, x2-x1);
    mvvline(y1, x1, 0, y2-y1);
    mvvline(y1, x2, 0, y2-y1);
    mvaddch(y1, x1, ACS_ULCORNER);
    mvaddch(y2, x1, ACS_LLCORNER);
    mvaddch(y1, x2, ACS_URCORNER);
    mvaddch(y2, x2, ACS_LRCORNER);
}

void drawCpuStatus(Bus *nes, uint8_t color_title = 1, uint8_t color_important_info = 2,uint8_t color_text=3)
{
    uint8_t x = 110;
    uint8_t y = 2;
    std::ostringstream ss;
    attron(COLOR_PAIR(color_title));
    ss << "CPU Status\n";
    move(y,x);
    addstr(ss.str().c_str());
    ss.str("");
    ss.clear();
    addstr(ss.str().c_str());

    rectansgle(y,x-1,y+2,x+40);
    ss << "[Status Register]: ";

    std::string red("\033[31m");
    std::string blue("\033[31m");
    std::string green("\033[32m");
    y++;move(y,x);
    addstr(ss.str().c_str());

    (((nes->_cpu._status_register & (CPU_6502::FLAGS6502::C) ) > 0) ? attron(COLOR_PAIR(color_important_info)) : attron(COLOR_PAIR(color_text))); addstr("C "); 
    (((nes->_cpu._status_register & (CPU_6502::FLAGS6502::Z) ) > 0) ? attron(COLOR_PAIR(color_important_info)) : attron(COLOR_PAIR(color_text))); addstr("Z "); 
    (((nes->_cpu._status_register & (CPU_6502::FLAGS6502::I) ) > 0) ? attron(COLOR_PAIR(color_important_info)) : attron(COLOR_PAIR(color_text))); addstr("I "); 
    (((nes->_cpu._status_register & (CPU_6502::FLAGS6502::B) ) > 0) ? attron(COLOR_PAIR(color_important_info)) : attron(COLOR_PAIR(color_text))); addstr("B "); 
    (((nes->_cpu._status_register & (CPU_6502::FLAGS6502::U) ) > 0) ? attron(COLOR_PAIR(color_important_info)) : attron(COLOR_PAIR(color_text))); addstr("U "); 
    (((nes->_cpu._status_register & (CPU_6502::FLAGS6502::V) ) > 0) ? attron(COLOR_PAIR(color_important_info)) : attron(COLOR_PAIR(color_text))); addstr("V "); 
    (((nes->_cpu._status_register & (CPU_6502::FLAGS6502::N) ) > 0) ? attron(COLOR_PAIR(color_important_info)) : attron(COLOR_PAIR(color_text))); addstr("N ");
    y++;y++;move(y,x);
    rectansgle(y,x-1,y+2,x+40);

//    y++;move(y,x);
    y+=1;move(y,x);

    attron(COLOR_PAIR(color_title));addstr("[X Register]: "); (nes->_cpu._x ?  attron(COLOR_PAIR(color_important_info)) : attron(COLOR_PAIR(color_text))); addstr("$"); addstr(hex(nes->_cpu._x, 2).c_str());
    y+=2;move(y,x);
    rectansgle(y,x-1,y+2,x+40);
    y+=1;move(y,x);attron(COLOR_PAIR(color_title));addstr("[Y Register]: "); (nes->_cpu._y ?  attron(COLOR_PAIR(color_important_info)) : attron(COLOR_PAIR(color_text))); addstr("$"); addstr(hex(nes->_cpu._y, 2).c_str());
    y+=2;move(y,x);
    rectansgle(y,x-1,y+2,x+40);
    y++;move(y,x);attron(COLOR_PAIR(color_title));addstr("[Accumulator Register]: "); (nes->_cpu._accumulator_register ?  attron(COLOR_PAIR(color_important_info)) : attron(COLOR_PAIR(color_text))); addstr("$"); addstr(hex(nes->_cpu._accumulator_register, 2).c_str());
    y+=2;move(y,x);
    rectansgle(y,x-1,y+2,x+40);
    y++;move(y,x);attron(COLOR_PAIR(color_title));addstr("[Stack Pointer]: "); (nes->_cpu._stack_pointer ?  attron(COLOR_PAIR(color_important_info)) : attron(COLOR_PAIR(color_text))); addstr("$"); addstr(hex(nes->_cpu._stack_pointer, 2).c_str());
    y+=2;move(y,x);
    rectansgle(y,x-1,y+2,x+40);
    y++;move(y,x);attron(COLOR_PAIR(color_title));addstr("[Program Counter]: "); (nes->_cpu._program_counter ?  attron(COLOR_PAIR(color_important_info)) : attron(COLOR_PAIR(color_text))); addstr("$"); addstr(hex(nes->_cpu._program_counter, 2).c_str());
    return;
    ss << (((nes->_cpu._status_register & (CPU_6502::FLAGS6502::Z) ) > 0) ? green : red) << "Z"; 
    ss << (((nes->_cpu._status_register & (CPU_6502::FLAGS6502::I) ) > 0) ? green : red) << "I"; 
    ss << (((nes->_cpu._status_register & (CPU_6502::FLAGS6502::D) ) > 0) ? green : red) << "D"; 
    ss << (((nes->_cpu._status_register & (CPU_6502::FLAGS6502::B) ) > 0) ? green : red) << "B"; 
    ss << (((nes->_cpu._status_register & (CPU_6502::FLAGS6502::U) ) > 0) ? green : red) << "U"; 
    ss << (((nes->_cpu._status_register & (CPU_6502::FLAGS6502::V) ) > 0) ? green : red) << "V"; 
    ss << (((nes->_cpu._status_register & (CPU_6502::FLAGS6502::N) ) > 0) ? green : red) << "N";
    ss << std::endl;

    std::string stop_color("\033[00m");
    std::string cyan("\033[36m");

    std::cout << "----CPU status------" << std::endl;
    // status register

    // other registers
    std::cout << blue << "[X Register]: " << (nes->_cpu._x ? cyan : stop_color )<<"$" << hex(nes->_cpu._x, 2) << stop_color<< std::endl;
    std::cout << blue << "[Y Register]: " << (nes->_cpu._y ? cyan : stop_color ) << "$" <<hex(nes->_cpu._y, 2) << stop_color<< std::endl;
    std::cout << blue << "[Accumulator Register]: " << (nes->_cpu._accumulator_register ? cyan : stop_color ) << "$"<< hex(nes->_cpu._accumulator_register, 2) << stop_color<< std::endl;
    std::cout << blue << "[Stack Pointer]: " << (nes->_cpu._stack_pointer ? cyan : stop_color ) << "$"<< hex(nes->_cpu._stack_pointer, 2) << stop_color<< std::endl;

    
}

void drawCurrentStatus(Bus *nes, uint16_t start, uint16_t end, int x, int y, uint8_t color_title = 1, uint8_t color_important_info = 7,uint8_t color_text=3)
{

    move(y,x);
    attron(COLOR_PAIR(color_title));addstr("CURRENT STATUS "); 
    auto disassembly = nes->_cpu.disassemble(start, end);

    for (const auto& [addr, instr] : disassembly) {
        if (addr == nes->_cpu._program_counter)
            attron(COLOR_PAIR(color_important_info));
        else
            attron(COLOR_PAIR(color_text));
        y++;move(y,x);addstr(instr.c_str());
    }
}


void printStringAtPosWithColors(std::string str, int x, int y, uint8_t  color_pair, uint8_t with_bold_effect)
{
    move(y,x);
    attron(COLOR_PAIR(color_pair));
    if (with_bold_effect)
        attron(A_BOLD);
    addstr(str.c_str());
    attroff(COLOR_PAIR(color_pair));
    if (with_bold_effect)
        attroff( A_BOLD);
}

void rectangle_ram(int y1, int x1, int y2, int x2)
{
    mvhline(y1, x1, 0, x2-x1);
    mvhline(y2, x1, 0, x2-x1);
    mvvline(y1, x1, 0, y2-y1);
    mvvline(y1, x2, 0, y2-y1);
    mvaddch(y1, x1, ACS_ULCORNER);
    mvaddch(y2, x1, ACS_LLCORNER);
    mvaddch(y1, x2, ACS_URCORNER);
    mvaddch(y2, x2, ACS_LRCORNER);
}

void drawRamUsageBox(Bus *nes, uint16_t x, uint16_t y, uint16_t ranges, uint8_t color_title = 2)
{
    int x_init = x;
    int size = nes->_ram.size();
    int limit = size / ranges;
    int lines = 8;
    int step_x = 5;
    int step_y = 3;
    size_t count = 0;
    char *str = NULL; 
//    std::string_view view
    move(y,x);
    attron(COLOR_PAIR(2));
    addstr("GLOBAL RAM OVERVIEW:");
    y+=3;
    rectansgle(y,x,y + (step_y * limit / lines), x + (step_x * lines));
//    addstr(std::to_string(limit).c_str());

    for (int i = 1; i < limit; i++) {
        move(y,x);
//        attron(COLOR_PAIR(color_title));
//        rectangle_ram(y, x, y+2, x+2);
//        addstr("0");
        auto begin = nes->_ram.begin() + (i-1) * ranges;
        auto end = nes->_ram.begin() + std::min((size_t)i * ranges, nes->_ram.size());
        count = std::count_if(begin, end, [](uint8_t v){return v != 0;});
        if (count)
            attron(COLOR_PAIR(11));
        else
            attron(COLOR_PAIR(10));
//        mvaddstr(y, x,"$00 ");
        addstr("$");addnstr(hex((i-1)*ranges,4).c_str(),2);
        move(y-1,x);addstr("   ");move(y+1,x);addstr("   ");move(y,x-1);addstr(" ");move(y,x+3);addstr(" ");move(y-1,x-1);addstr(" ");move(y+1,x+3);addstr(" ");move(y-1,x+3);addstr(" ");move(y+1,x-1);addstr(" ");
        x+=step_x;
        if (i % lines == 0) {
            y+=step_y;
            x -= (lines * step_x);
        }

    }
    y+=4;
    move(y,x_init);
    attron(COLOR_PAIR(2));
    addstr("Press r : reset");
    y++;move(y,x_init);
    addstr("Press q : Exit Program");
    y++;move(y,x_init);
    addstr("Press Any other Key : Continue Program");


}

void drawNes(Bus *nes, uint16_t offset)
{
    printStringAtPosWithColors("-----NES CPU and RAM-----", COLS / 2 - 25 , 0, 2, 1);

    drawRamRange(nes, 0x0000, 16, 16, 0, 2, 2);
    drawRamRange(nes, offset, 16, 16, 0, 20, 2);
    drawRamUsageBox(nes,60, 3, 1000);
    // Draw CPU Status
    drawCpuStatus(nes);
    // Draw Current Status
    drawCurrentStatus(nes, nes->_cpu._program_counter-10, nes->_cpu._program_counter + 20, 110,20);
    
}

int main(int argc, char const *argv[])
{
    char input = 0;

    initscr();
    start_color();
    initNcursesColors();
    keypad(stdscr, TRUE);
    cbreak();
    noecho();

    /* initialize colors */

    if (has_colors() == FALSE) {
        endwin();
        puts("Your terminal does not support color");
        return(1);
    }

    Bus nes;
//    std::string input("");
    uint16_t offset = 0x6000;

    std::stringstream program_object_code;

    program_object_code << "A2 0A 8E 00 00 A2 03 8E 01 00 AC 00 00 A9 00 18 6D 01 00 88 D0 FA 8D 02 00 EA EA EA";
	uint16_t nOffset = offset;
	while (!program_object_code.eof())
	{
		std::string b;
		program_object_code >> b;
		nes._ram[nOffset++] = (uint8_t)std::stoul(b, nullptr, 16);
	}

    // The program counter will start at offset. So we have to indicate in the ram at 0xFFFE and 0xFFFF how offset splited
    // Because when a BRK happens, the program counter is reset to the value located at these places
    nes._cpu._program_counter = offset;

    // low byte
    nes._ram[0xFFFE] = (offset  & 0xFF);
    // high byte
    nes._ram[0xFFFF] = offset >> 8;

    // we do the same for reset
        // low byte
    nes._ram[0xFFFC] = (offset  & 0xFF);
    // high byte
    nes._ram[0xFFFD] = offset >> 8;

    while (1) {
        drawNes(&nes, offset);
        refresh();
        input = getch();

        if (input == 'q') {
            break;
        }
        if (input == 'r') {
            nes._cpu.reset();
        } else {
            nes._cpu.clock();
            while (!nes._cpu.complete())
                nes._cpu.clock();
        }

    }

    endwin();
    
    return 0;
}
