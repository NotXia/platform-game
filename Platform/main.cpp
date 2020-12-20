﻿#include <iostream>
#include <conio.h>
#include <cstdlib>
#include <ctime>
#include "Screen.hpp"
#include "Map.hpp"
#include "Player.hpp"
#include "colors.h"
#include "Weapon.hpp"
#include "Bullet.hpp"
#include "EnemyList.hpp"
using namespace std;

int main() {
    srand(time(0));

    Weapon test = Weapon(
        Pixel(char(196), FG_GREEN | BG_CYAN, false), 
        Pixel(char(196), FG_GREEN | BG_CYAN, false),
        Bullet(Pixel('-', FG_BLACK | BACKGROUND_DEFAULT, false), 100, 10)
    );

    int difficulty = 1;
    int max_enemies = 2;
    Screen screen = Screen();
    Map *map = new Map(NULL, 3, difficulty);
    Player player = Player(MAX_LIFE, Pixel('<', PLAYER_HEAD_COLOR, true), Pixel('>', PLAYER_HEAD_COLOR, true), Pixel(char(219), PLAYER_BODY_COLOR, true), map->getLeftPosition(), test);
    //Position head_position;
    EnemyList enemylist;
    BulletList bulletlist;

    
    screen.init();

    screen.write_game_area(map);
    screen.write_enemies(map->getEnemyList());
    screen.write_entity(player);

    while (true) {

        /* ----------------------------
           INIZIO GESTIONE GIOCATORE   
        ---------------------------- */
        if (_kbhit()) {
            char ch = _getch();
            if (ch == 'a' && player.getCanMove() && (!map->isSolidAt(player.getFrontPosition()) || (player.getDirection() == DIRECTION_RIGHT))) {
                screen.resetTerrain(map, player.getBodyPosition());
                screen.resetTerrain(map, player.getHeadPosition());
                player.goLeft();
                screen.write_entity(player);

                if (player.getBodyPosition().getX() <= 1) {
                    if (!map->prevNull()) {
                        map = map->gotoPrevious(player.getBodyPosition());
                        screen.write_game_area(map);
                        screen.write_enemies(map->getEnemyList());
                        player.setPosition(map->getRightPosition());
                        screen.write_entity(player);
                    }
                }
            }
            else if (ch == 'd' && player.getCanMove() && (!map->isSolidAt(player.getFrontPosition()) || (player.getDirection() == DIRECTION_LEFT))) {
                screen.resetTerrain(map, player.getBodyPosition());
                screen.resetTerrain(map, player.getHeadPosition());
                player.goRight();
                screen.write_entity(player);

                if (player.getBodyPosition().getX() >= GAME_WIDTH) {
                    map = map->gotoNext(player.getBodyPosition(), max_enemies, difficulty);
                    screen.write_game_area(map);
                    screen.write_enemies(map->getEnemyList());
                    player.setPosition(map->getLeftPosition());
                    screen.write_entity(player);
                }
            }
            else if (ch == 'w' && player.getCanMove()) {
                if (player.isOnTerrain()) {
                    player.initJump();
                }
            }
            else if (ch == 's' && player.getCanMove()) {
                if (map->isSolidAt(Position(player.getBodyPosition().getX(), player.getBodyPosition().getY()+1)) &&
                    !map->isSolidAt(Position(player.getBodyPosition().getX(), player.getBodyPosition().getY()+2)) &&
                    !map->isSolidAt(Position(player.getBodyPosition().getX(), player.getBodyPosition().getY()+3))) {

                    player.setOnTerrain(false);
                    screen.resetTerrain(map, player.getBodyPosition());
                    screen.resetTerrain(map, player.getHeadPosition());
                    player.fall();
                }
            }
            else if (ch == 32) {
                if (player.isOnTerrain()) {
                    map->addBullet(player.attack());

                    player.resetWeaponLoop();
                    screen.write_at(player.getWeapon().getTexture(player.getDirection()), player.getFrontPosition());
                }
            }
        } // if (_kbhit())

        /*** Gestione salto ***/
        if (player.isJumping() && player.canJump()) { 
            // Interrompe il salto se raggiunge il soffitto
            if (player.getHeadPosition().getY() <= 1 || (map->isSolidAt(Position(player.getHeadPosition().getX(), player.getHeadPosition().getY()-1)) && map->isSolidAt(Position(player.getHeadPosition().getX(), player.getHeadPosition().getY()-2)))) {
                player.stopJump();
            }
            else {
                player.resetJumpLoopCounter();
                screen.resetTerrain(map, player.getBodyPosition());
                screen.resetTerrain(map, player.getHeadPosition());

                player.jump();

                screen.write_entity(player);
            }
        }
        /*** Gestione caduta ***/
        else if (!player.isJumping() && !map->isSolidAt(Position(player.getBodyPosition().getX(), player.getBodyPosition().getY()+1)) && player.canFall()) {
            player.setOnTerrain(false);
            player.resetFallLoopCounter();
            screen.resetTerrain(map, player.getBodyPosition());
            screen.resetTerrain(map, player.getHeadPosition());

            player.fall();

            screen.write_entity(player);
        }
        /*** Gestione arrivo a terra ***/
        else if (!player.isJumping() && map->isSolidAt(Position(player.getBodyPosition().getX(), player.getBodyPosition().getY()+1))) {
            player.setOnTerrain(true);
        }
        /*** Gestione animazione arma ***/
        if (player.endWeaponDisplay()) {
            screen.resetTerrain(map, player.getFrontPosition());
            player.setCanMove(true);
        }

        player.incJumpLoopCounter();
        player.incFallLoopCounter();
        player.incWeaponLoop();
        /* FINE GESTIONE GIOCATORE   
        -------------------------- */



        /* -----------------------------
           INIZIO GESTIONE PROIETTILI   
        ----------------------------- */
        bulletlist = map->getBulletList();
        bulletlist.initIter();

        while (!bulletlist.isNull()) {
            enemylist = map->getEnemyList();
            Bullet bullet = bulletlist.getCurrent();

            if (bullet.canRefresh()) {
                screen.resetTerrain(map, bullet.getPosition());

                if (bullet.isHostile()) {

                }
                else {
                    if (enemylist.pointAt(bullet.getPosition())) {
                        Enemy hit_enemy = enemylist.getCurrent();
                        hit_enemy.take_damage(bullet.hit());

                        if (hit_enemy.isDead()) {
                            screen.resetTerrain(map, hit_enemy.getHeadPosition());
                            screen.resetTerrain(map, hit_enemy.getBodyPosition());
                        }
                        enemylist.updateCurrent(hit_enemy);
                        map->setEnemyList(enemylist);
                    }
                }
                
                if (bullet.travel()) {
                    screen.write_at(bullet.getTexture(), bullet.getPosition());
                }
            }

            bullet.incRefresh();

            bulletlist.updateCurrent(bullet);
            map->setBulletList(bulletlist);
        }
        /*  FINE GESTIONE PROIETTILI   
        --------------------------- */



        /* --------------------------
           INIZIO GESTIONE UMARELL   
        -------------------------- */
        enemylist = map->getEnemyList();
        enemylist.initIter();

        while (!enemylist.isNull()) {
            Enemy enemy = enemylist.getCurrent();

            if (enemy.canRefresh()) {
                enemy.search4Player(player);

                int action = enemy.getAction(map, player);
                if (action == ACTION_GO_RIGHT) {
                    screen.resetTerrain(map, enemy.getBodyPosition());
                    screen.resetTerrain(map, enemy.getHeadPosition()); 
                    if (player.getCanMove() && (!map->isSolidAt(enemy.getFrontPosition()) && player.getBodyPosition().getX() != enemy.getFrontPosition().getX() || (enemy.getDirection() == DIRECTION_LEFT))) {
                        enemy.goRight();
                    }
                    screen.write_entity(enemy);
                }
                else if (action == ACTION_GO_LEFT) {
                    screen.resetTerrain(map, enemy.getBodyPosition());
                    screen.resetTerrain(map, enemy.getHeadPosition()); 
                    if (player.getCanMove() && (!map->isSolidAt(enemy.getFrontPosition()) && player.getBodyPosition().getX() != enemy.getFrontPosition().getX() || (enemy.getDirection() == DIRECTION_RIGHT))) {
                        enemy.goLeft();
                    }
                    screen.write_entity(enemy);
                }
                else if (action == ACTION_JUMP) {
                    if (enemy.isOnTerrain()) {
                        enemy.initJump();
                    }
                }
                else if (action == ACTION_FALL) {
                    if (map->isSolidAt(Position(enemy.getBodyPosition().getX(), enemy.getBodyPosition().getY()+1)) &&
                        !map->isSolidAt(Position(enemy.getBodyPosition().getX(), enemy.getBodyPosition().getY()+2)) &&
                        !map->isSolidAt(Position(enemy.getBodyPosition().getX(), enemy.getBodyPosition().getY()+3))) {

                        enemy.setOnTerrain(false);
                        screen.resetTerrain(map, enemy.getBodyPosition());
                        screen.resetTerrain(map, enemy.getHeadPosition());
                        enemy.fall();
                    }
                }
                else if (action == ACTION_ATTACK) {

                }
            }

            /*** Gestione salto ***/
            if (enemy.isJumping() && enemy.canJump()) {
                // Interrompe il salto se raggiunge il soffitto
                if (enemy.getHeadPosition().getY() <= 1 || 
                     (map->isSolidAt(Position(enemy.getHeadPosition().getX(), enemy.getHeadPosition().getY()-1)) && 
                     map->isSolidAt(Position(enemy.getHeadPosition().getX(), enemy.getHeadPosition().getY()-2))) || 
                     player.getBodyPosition().equals(Position(enemy.getHeadPosition().getX(), enemy.getHeadPosition().getY()-2))
                    ) {
                    enemy.stopJump();
                }
                else {
                    enemy.resetJumpLoopCounter();
                    screen.resetTerrain(map, enemy.getBodyPosition());
                    screen.resetTerrain(map, enemy.getHeadPosition());

                    enemy.jump();

                    screen.write_entity(enemy);
                }
            }
            /*** Gestione caduta ***/
            else if (!enemy.isJumping() && 
                (!map->isSolidAt(Position(enemy.getBodyPosition().getX(), enemy.getBodyPosition().getY()+1)) &&
                 !player.getHeadPosition().equals(Position(enemy.getBodyPosition().getX(), enemy.getBodyPosition().getY()+1))) && 
                 enemy.canFall()
                ) {
                enemy.setOnTerrain(false);
                enemy.resetFallLoopCounter();
                screen.resetTerrain(map, enemy.getBodyPosition());
                screen.resetTerrain(map, enemy.getHeadPosition());

                enemy.fall();

                screen.write_entity(enemy);
            }
            /*** Gestione arrivo a terra ***/
            else if (!enemy.isJumping() && 
                (map->isSolidAt(Position(enemy.getBodyPosition().getX(), enemy.getBodyPosition().getY()+1)) || 
                player.getHeadPosition().equals(Position(enemy.getBodyPosition().getX(), enemy.getBodyPosition().getY()+1)))
                ) {
                enemy.setOnTerrain(true);
            }
            
            /*** Gestione collisione con proiettili ***/
            bulletlist = map->getBulletList();
            if (bulletlist.pointAt(enemy.getHeadPosition(), enemy.getBodyPosition())) {
                Bullet hit_bullet = bulletlist.getCurrent();

                enemy.take_damage(hit_bullet.hit());
                if (enemy.isDead()) {
                    screen.resetTerrain(map, enemy.getHeadPosition());
                    screen.resetTerrain(map, enemy.getBodyPosition());
                }

                bulletlist.updateCurrent(hit_bullet);
                map->setBulletList(bulletlist);
            }

            enemy.incJumpLoopCounter();
            enemy.incFallLoopCounter();
            enemy.incRefresh();

            enemylist.updateCurrent(enemy);
            map->setEnemyList(enemylist);
        }
        /* FINE GESTIONE UMARELL   
        ------------------------ */
        
    } // while (true)
}
