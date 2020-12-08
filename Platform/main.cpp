#include <iostream>
#include <conio.h>
#include <cstdlib>
#include <ctime>
#include "Screen.hpp"
#include "Map.hpp"
#include "Player.hpp"
using namespace std;

int main() {
    srand(time(0));

    Screen screen = Screen();
    Map *map = new Map;
    *map = Map(NULL); 
    Pixel curr_terrain[GAME_WIDTH][GAME_HEIGHT]; // TODO: Renderlo un puntatore oppure ridurre la dimensione del gioco
    Player player = Player(MAX_LIFE, Pixel('<', 0x80, true), Pixel('>', 0x80, true), Pixel(char(219), 0x80, true), NULL, map->getLeftPosition());

    screen.init();

    map->getTerrain(curr_terrain);
    screen.write_game_area(curr_terrain);
    screen.write_entity_right(player);

    while (true) {
        if (_kbhit()) {
            char ch = _getch();
            switch (ch) {
                case 'a':
                    player.goLeft();
                    screen.write_game_area(curr_terrain);
                    screen.write_entity_left(player);
                    if (player.getPosition().getX() <= 1) {
                        if (map->getPrev() != NULL) {
                            map = map->getPrev();
                            map->getTerrain(curr_terrain);
                            player.setPosition(map->getRightPosition());
                        }
                    }
                    break;

                case 'd':
                    player.goRight();
                    screen.write_game_area(curr_terrain);
                    screen.write_entity_right(player);
                    if (player.getPosition().getX() >= GAME_WIDTH) {
                        map = map->getNext();
                        map->getTerrain(curr_terrain);
                        player.setPosition(map->getLeftPosition());
                    }
                    
                    break;
            }
        }
    }
}
