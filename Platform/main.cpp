#include <iostream>
#include <conio.h>
#include <cstdlib>
#include <ctime>
#include "Screen.hpp"
#include "Map.hpp"
#include "Player.hpp"
#include "colors.h"
using namespace std;

int main() {
    srand(time(0));

    Screen screen = Screen();
    Map *map = new Map;
    *map = Map(NULL); 
    Pixel curr_terrain[GAME_WIDTH][GAME_HEIGHT]; // TODO: Renderlo un puntatore oppure ridurre la dimensione del gioco
    Player player = Player(MAX_LIFE, Pixel('<', PLAYER_HEAD_COLOR, true), Pixel('>', PLAYER_HEAD_COLOR, true), Pixel(char(219), PLAYER_BODY_COLOR, true), NULL, map->getLeftPosition());
    Position head_position;
    int loop = 0;

    screen.init();

    map->getTerrain(curr_terrain);
    screen.write_game_area(curr_terrain);
    screen.write_entity_right(player);

    while (true) {
        if (_kbhit()) {
            char ch = _getch();
            if (ch == 'a') {
                screen.resetTerrain(curr_terrain, player.getBodyPosition());
                screen.resetTerrain(curr_terrain, player.getHeadPosition());
                player.goLeft();
                screen.write_entity_left(player);

                if (player.getBodyPosition().getX() <= 1) {
                    if (map->gotoPrevious(player.getBodyPosition()) != NULL) {
                        map = map->gotoPrevious(player.getBodyPosition());
                        map->getTerrain(curr_terrain);
                        screen.write_game_area(curr_terrain);
                        player.setPosition(map->getRightPosition());
                        screen.write_entity_left(player);
                    }
                }
            }
            else if (ch == 'd') {
                screen.resetTerrain(curr_terrain, player.getBodyPosition());
                screen.resetTerrain(curr_terrain, player.getHeadPosition());
                player.goRight();
                screen.write_entity_right(player);

                if (player.getBodyPosition().getX() >= GAME_WIDTH) {
                    map = map->gotoNext(player.getBodyPosition());
                    map->getTerrain(curr_terrain);
                    screen.write_game_area(curr_terrain);
                    player.setPosition(map->getLeftPosition());
                    screen.write_entity_right(player);
                }
            }
            else if (ch == 'w') {
                if (player.getCanJump()) {
                    player.initJump();
                }
            }
        } // if (_kbhit())

        /*** Gestione salto ***/
        if (player.isJumping() && loop >= JUMP_SPEED) { 
            // Interrompe il salto se raggiunge il soffitto
            if (player.getHeadPosition().getY() <= 1) {
                player.stopJump();
            }
            else {
                loop = 0;
                screen.resetTerrain(curr_terrain, player.getBodyPosition());
                screen.resetTerrain(curr_terrain, player.getHeadPosition());

                player.jump();

                if (player.getDirection() == DIRECTION_LEFT) { screen.write_entity_left(player); }
                else { screen.write_entity_right(player); }
            }
        }
        /*** Gestione caduta ***/
        else if (!player.isJumping() && !curr_terrain[player.getBodyPosition().getX()-1][player.getBodyPosition().getY()].isSolid() && loop >= FALL_SPEED) {
            player.setCanJump(false);
            loop = 0;
            screen.resetTerrain(curr_terrain, player.getBodyPosition());
            screen.resetTerrain(curr_terrain, player.getHeadPosition());

            player.fall();

            if (player.getDirection() == DIRECTION_LEFT) { screen.write_entity_left(player); }
            else { screen.write_entity_right(player); }
        }
        /*** Gestione arrivo a terra ***/
        else if (!player.isJumping() && curr_terrain[player.getBodyPosition().getX()-1][player.getBodyPosition().getY()].isSolid()) {
            loop = 0;
            player.setCanJump(true);
        }

        loop++;
    }
}
