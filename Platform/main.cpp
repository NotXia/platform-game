#include <iostream>
#include <windows.h>
#include <conio.h>
#include <ctime>
#include <cstdlib>
#include "Screen.hpp"
#include "Map.hpp"
using namespace std;


int main() {
    srand(time(0));

    Screen screen = Screen();
    Map* map = new Map;
    *map = Map(NULL); // TODO: Renderlo un puntatore oppure ridurre la dimensione del gioco
    Pixel curr_terrain[GAME_WIDTH][GAME_HEIGHT];

    screen.init();

    map->getTerrain(curr_terrain);
    screen.write_game_area(curr_terrain);

    while (true) {
        if (_kbhit()) {
            char ch = _getch();
            switch (ch) {
                case 'a':
                    if(map->getPrev() != NULL)
                        map = map->getPrev();
                    break;

                case 'd':
                    map = map->getNext();
                    break;
            }

            map->getTerrain(curr_terrain);
            screen.write_game_area(curr_terrain);
        }
    }
}
