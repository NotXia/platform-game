#include <iostream>
#include <conio.h>
#include <cstdlib>
#include <ctime>
#include "Screen.hpp"
#include "Map.hpp"
#include "Player.hpp"
#include "colors.h"
#include "Weapon.hpp"
#include "Bullet.hpp"
using namespace std;

int main() {
    srand(time(0));

    Weapon test = Weapon(
        Pixel(char(196), FG_GREEN | BG_CYAN, false), Pixel(char(196), FG_GREEN | BG_CYAN, false),
        Bullet(Pixel(char(153), FG_WHITE | BACKGROUND_DEFAULT, false), 10, 10, 10)
    );

    Screen screen = Screen();
    Map *map = new Map(NULL);
    //*map = Map(NULL); 
    Player player = Player(MAX_LIFE, Pixel('<', PLAYER_HEAD_COLOR, true), Pixel('>', PLAYER_HEAD_COLOR, true), Pixel(char(219), PLAYER_BODY_COLOR, true), &test, map->getLeftPosition());
    Position head_position;

    

    screen.init();

    screen.write_game_area(map);
    screen.write_entity_right(player);

    while (true) {
        if (_kbhit()) {
            char ch = _getch();
            if (ch == 'a' && player.getCanMove()) {
                screen.resetTerrain(map, player.getBodyPosition());
                screen.resetTerrain(map, player.getHeadPosition());
                player.goLeft();
                screen.write_entity_left(player);

                if (player.getBodyPosition().getX() <= 1) {
                    if (!map->prevNull()) {
                        map = map->gotoPrevious(player.getBodyPosition());
                        screen.write_game_area(map);
                        player.setPosition(map->getRightPosition());
                        screen.write_entity_left(player);
                    }
                }
            }
            else if (ch == 'd' && player.getCanMove()) {
                screen.resetTerrain(map, player.getBodyPosition());
                screen.resetTerrain(map, player.getHeadPosition());
                player.goRight();
                screen.write_entity_right(player);

                if (player.getBodyPosition().getX() >= GAME_WIDTH) {
                    map = map->gotoNext(player.getBodyPosition(), 0);
                    screen.write_game_area(map);
                    player.setPosition(map->getLeftPosition());
                    screen.write_entity_right(player);
                }
            }
            else if (ch == 'w' && player.getCanMove()) {
                if (player.isOnTerrain()) {
                    player.initJump();
                }
            }
            else if (ch == 's' && player.getCanMove()) {
                if (map->getMapAt(Position(player.getBodyPosition().getX(), player.getBodyPosition().getY()+1)).isSolid() &&
                    !map->getMapAt(Position(player.getBodyPosition().getX(), player.getBodyPosition().getY()+2)).isSolid() &&
                    !map->getMapAt(Position(player.getBodyPosition().getX(), player.getBodyPosition().getY()+3)).isSolid()) {

                    player.setOnTerrain(false);
                    screen.resetTerrain(map, player.getBodyPosition());
                    screen.resetTerrain(map, player.getHeadPosition());
                    player.fall();
                }
            }
            else if (ch == 32) {
                if (player.isOnTerrain()) {
                    player.attack();

                    player.resetWeaponLoop();
                    screen.write_at(player.getWeapon()->getTexture(player.getDirection()), player.getFrontPosition());
                }
            }
        } // if (_kbhit())

        /*** Gestione salto ***/
        if (player.isJumping() && player.canJump()) { 
            // Interrompe il salto se raggiunge il soffitto
            if (player.getHeadPosition().getY() <= 1) {
                player.stopJump();
            }
            else {
                player.resetJumpLoopCounter();
                screen.resetTerrain(map, player.getBodyPosition());
                screen.resetTerrain(map, player.getHeadPosition());

                player.jump();

                if (player.getDirection() == DIRECTION_LEFT) { screen.write_entity_left(player); }
                else { screen.write_entity_right(player); }
            }
        }
        /*** Gestione caduta ***/
        else if (!player.isJumping() && !map->getMapAt(Position(player.getBodyPosition().getX(), player.getBodyPosition().getY()+1)).isSolid() && player.canFall()) {
            player.setOnTerrain(false);
            player.resetFallLoopCounter();
            screen.resetTerrain(map, player.getBodyPosition());
            screen.resetTerrain(map, player.getHeadPosition());

            player.fall();

            if (player.getDirection() == DIRECTION_LEFT) { screen.write_entity_left(player); }
            else { screen.write_entity_right(player); }
        }
        /*** Gestione arrivo a terra ***/
        else if (!player.isJumping() && map->getMapAt(Position(player.getBodyPosition().getX(), player.getBodyPosition().getY()+1)).isSolid()) {
            player.setOnTerrain(true);
        }

        if (player.endWeaponDisplay()) {
            screen.resetTerrain(map, player.getFrontPosition());
            player.setCanMove(true);
        }

        player.incJumpLoopCounter();
        player.incFallLoopCounter();
        player.incWeaponLoop();
    }
}
