#include <iostream>
#include <windows.h>
#include <conio.h>
#include <ctime>
#include <cstdlib>
#include "Screen.hpp"
using namespace std;

int main() {
    srand(time(0));

    Screen screen = Screen();

    screen.init();

    /*HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD destCoord;

    system("cls");

    while (true) {
        if (_kbhit()) {
            destCoord.X = 10;
            destCoord.Y = 4;
            SetConsoleCursorPosition(console, destCoord);

            char ch = _getch();
            SetConsoleTextAttribute(console, rand()%555 | rand()%555);
            cout <<ch <<ch <<ch <<endl;
            SetConsoleTextAttribute(console, 7);
        }
    }*/
}
