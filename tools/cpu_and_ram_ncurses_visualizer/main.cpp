#include <iostream>
#include <ncurses.h>

    

int main(int argc, char const *argv[])
{
    initscr();
    keypad(stdscr, TRUE);
    cbreak();
    noecho();

    /* initialize colors */

    if (has_colors() == FALSE) {
        endwin();
        puts("Your terminal does not support color");
        exit(1);
    }

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
    
    return 0;             // Restaure les paramètres par défaut du terminal
    return 0;
}
