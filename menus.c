#include "menus.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <windows.h>
#include <conio.h>

char *string =
        "\n \t\t 000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000"
        "\n \t\t 0                                                                                                    0"
        "\n \t\t 0                                                                                                    0"
        "\n \t\t 0    666666666666    111111111111    222222222222    3333333333      444444444444      5555555555    0"
        "\n \t\t 0        6666        1111                2222        3333    3333        4444        55555           0"
        "\n \t\t 0        6666        111111111111        2222        333333333333        4444        55555555        0"
        "\n \t\t 0        6666        111111111111        2222        33333333            4444            55555555    0"
        "\n \t\t 0        6666        1111                2222        3333    3333        4444               55555    0"
        "\n \t\t 0        6666        111111111111        2222        3333    3333    444444444444    5555555555      0"
        "\n \t\t 0                                                                                                    0"
        "\n \t\t 0                                                                                                    0";

char *string2 =
        "\n \t\t 0                                                                                                    0"
        "\n \t\t 0                                              Ver 1.1                                               0"
        "\n \t\t 0                                                                                                    0"
        "\n \t\t 0                                                                                                    0"
        "\n \t\t 0                                          by Gioele Bucci                                           0"
        "\n \t\t 0                                                                                                    0"
        "\n \t\t 0                                                                                                    0"
        "\n \t\t 0                                      Press any key to start:                                       0"
        "\n \t\t 0                                                                                                    0"
        "\n \t\t 0                                                                                                    0"
        "\n \t\t 0                                                                                                    0"
        "\n \t\t 0                                                         https://github.com/GioeleBucci/CMD-Tetris  0"
        "\n \t\t 0                                                                                                    0"
        "\n \t\t 000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000\n\n";

void printTitleScreen() {
    system("cls");
    printf("\n\n\n\n\n");
    for (int i = 0; i < strlen(string); ++i) {
        printf("\x1b[3%dm%c\x1b[0m", string[i] == '0' ? 7 : atoi(&string[i]),
               isdigit(string[i]) ? 219 : string[i]);
    }
    for (int i = 0; i < strlen(string2); ++i) {
        printf("%c",string2[i] == '0' ? 219 : string2[i]);
    }
    Sleep(500);
    getch();
}














