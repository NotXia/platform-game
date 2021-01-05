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
#include "EnemyList.hpp"
#include "WeaponContainer.hpp"
using namespace std;

const char KEY_SPACEBAR = 32;

int main() {
    srand(time(0));

    WeaponContainer *weapon_container = new WeaponContainer();
    weapon_container->initForPlayer();

    int max_enemies = 2;
    Screen screen = Screen();
    Map *map = new Map(NULL, 2);
    Player player = Player(MAX_LIFE, 
        Pixel(char(17), PLAYER_HEAD_COLOR_FG, BACKGROUND_DEFAULT, true),
        Pixel(char(16), PLAYER_HEAD_COLOR_FG, BACKGROUND_DEFAULT, true),
        Pixel(char(219), PLAYER_BODY_COLOR_FG, BACKGROUND_DEFAULT, true), map->getLeftPosition(), weapon_container->getRandomTier3());
    EnemyList enemylist;
    BulletList bulletlist;

    Bonus *curr_bonus = NULL;
    NPC *curr_npc = NULL;
    bool hasMoved;
    bool showNPCs = true;
    char weapon_name[STRING_LEN];
    
    screen.init();

    screen.write_game_area(map);
    screen.write_entity(map, player);

    player.getWeapon().getName(weapon_name);
    screen.write_weaponbox(weapon_name);
    screen.write_ammobox(player.getWeapon().getCurrAmmo());
    screen.write_money(player.getMoney());
    screen.write_hp(player.getHealth());


    while (!player.isDead()) {
    //while (true) {
        if (screen.canRotateWeaponbox()) {
            screen.rotate_weaponbox();
        }

        /* ----------------------------
           INIZIO GESTIONE GIOCATORE   
        ---------------------------- */
        hasMoved = false;

        if (_kbhit()) {
            char ch = _getch();
            if (ch == 'a' || ch == 'A') {
                hasMoved = true;
                if (player.getCanMove() && (!map->isSolidAt(player.getFrontPosition()) || (player.getDirection() == DIRECTION_RIGHT))) {
                    screen.resetTerrain(map, player.getBodyPosition());
                    screen.resetTerrain(map, player.getHeadPosition());
                    player.goLeft();
                    screen.write_entity(map, player);

                    if (player.getBodyPosition().getX() <= 1) {
                        if (!map->prevNull()) {
                            map = map->gotoPrevious(player.getBodyPosition());
                            screen.write_game_area(map);
                            player.setPosition(map->getRightPosition());
                            screen.write_entity(map, player);
                            showNPCs = true;
                        }
                    }
                }
            }
            else if (ch == 'd' || ch == 'D' ) {
                hasMoved = true;
                if (player.getCanMove() && (!map->isSolidAt(player.getFrontPosition()) || (player.getDirection() == DIRECTION_LEFT))) {
                    screen.resetTerrain(map, player.getBodyPosition());
                    screen.resetTerrain(map, player.getHeadPosition());
                    player.goRight();
                    screen.write_entity(map, player);

                    if (player.getBodyPosition().getX() >= GAME_WIDTH) {
                        map = map->gotoNext(player.getBodyPosition(), max_enemies);
                        screen.write_game_area(map);
                        player.setPosition(map->getLeftPosition());
                        screen.write_entity(map, player);
                        showNPCs = true;
                    }
                }
            }
            else if (ch == 'w' || ch == 'W' ) {
                hasMoved = true;
                if (player.getCanMove() && player.isOnTerrain()) {
                    player.initJump();
                }
            }
            else if (ch == 's' || ch == 'S') {
                hasMoved = true;
                if (player.getCanMove() &&
                     map->isSolidAt(Position(player.getBodyPosition().getX(), player.getBodyPosition().getY()+1)) &&
                    !map->isSolidAt(Position(player.getBodyPosition().getX(), player.getBodyPosition().getY()+2)) &&
                    !map->isSolidAt(Position(player.getBodyPosition().getX(), player.getBodyPosition().getY()+3)) ) {

                    player.setOnTerrain(false);
                    screen.resetTerrain(map, player.getBodyPosition());
                    screen.resetTerrain(map, player.getHeadPosition());
                    player.fall();
                }
            }
            else if (ch == KEY_SPACEBAR) { // Spacebar
                if (player.isOnTerrain()) {
                    if (player.canAttack()) {
                        map->addBullet(player.attack());

                        screen.write_at(map, player.getWeapon().getTexture(player.getDirection()), player.getFrontPosition());
                    }
                    else {
                        if (player.canReload() && !player.getWeapon().hasAmmo()) {
                            player.reload();
                        }
                    }
                }
                screen.write_ammobox(player.getWeapon().getCurrAmmo());
            }
            else if (ch == 'r' || ch == 'R') {
                player.reload();
                screen.write_ammobox(player.getWeapon().getCurrAmmo());
            }

            // Se hasMoved è true, il giocatore si è mosso dall'eventuale bonus/NPC su cui si trovava
            if (hasMoved) {
                curr_bonus = NULL;
                curr_npc = NULL;
                screen.write_textbox("");
            }

            // Gestione bonus di tipo arma
            if (curr_bonus != NULL) {
                if (ch == 'e' || ch == 'E') {
                    Weapon old = player.getWeapon();
                    player.setWeapon(curr_bonus->getWeapon());

                    BonusList bonuslist = map->getBonusList();
                    bonuslist.pointAt(player.getBodyPosition());
                    *curr_bonus = bonuslist.getCurrent();
                    curr_bonus->setWeapon(old);
                    bonuslist.updateCurrent(*curr_bonus);
                    map->setBonusList(bonuslist);

                    screen.write_textbox_weaponbonus(curr_bonus->getWeapon(), player.getWeapon());
                    player.getWeapon().getName(weapon_name);
                    screen.write_weaponbox(weapon_name);
                    screen.write_ammobox(player.getWeapon().getCurrAmmo());
                }
            }

            // Gestione NPC
            if (curr_npc != NULL) {
                if (curr_npc->getType() == NPC_HOSPITAL) {
                    if (ch == 'e' || ch == 'E') {
                        if (player.getHealth() != MAX_LIFE) {
                            if (player.getMoney() >= curr_npc->getPriceHP()) {
                                player.decMoney(curr_npc->getPriceHP());
                                player.heal(1);
                                screen.write_hp(player.getHealth());
                                screen.write_money(player.getMoney());
                                char name[STRING_LEN];
                                curr_npc->getName(name);
                                screen.write_textbox_npc_hp(*curr_npc, player.getMissingHp());
                            }
                        }
                    }
                    else if (ch == 'q' || ch == 'Q') {
                        if (player.getMoney() >= curr_npc->getPriceHP()*(MAX_LIFE-player.getHealth())) {
                            player.decMoney(curr_npc->getPriceHP() *(MAX_LIFE-player.getHealth()));
                            player.heal(MAX_LIFE-player.getHealth());
                            screen.write_hp(player.getHealth());
                            screen.write_money(player.getMoney());
                            char name[STRING_LEN];
                            curr_npc->getName(name);
                            screen.write_textbox_npc_hp(*curr_npc, player.getMissingHp());
                        }
                    }
                }
                else if (curr_npc->getType() == NPC_WEAPONSHOP) {
                    if (curr_npc->getWeaponNumber() > 0) {
                        if (ch == 'o' || ch == 'O') {
                            curr_npc->setCurrSelected(curr_npc->getCurrSelected() - 1);
                            screen.write_textbox_npc_weapon(*curr_npc, player.getWeapon());
                        }
                        else if (ch == 'p' || ch == 'P') {
                            curr_npc->setCurrSelected(curr_npc->getCurrSelected() + 1);
                            screen.write_textbox_npc_weapon(*curr_npc, player.getWeapon());
                        }
                        else if (ch == 'e' || ch == 'E') {
                            if (player.getMoney() >= curr_npc->getCurrWeaponPrice()) {
                                player.decMoney(curr_npc->getCurrWeaponPrice());
                                screen.write_money(player.getMoney());
                                player.setWeapon(curr_npc->getCurrWeapon());
                                char weapon_name[STRING_LEN];
                                player.getWeapon().getName(weapon_name);
                                screen.write_weaponbox(weapon_name);
                                screen.write_ammobox(player.getWeapon().getCurrAmmo());
                                curr_npc->deleteCurrentWeapon();
                                NPCList npclist = map->getNPCList();
                                npclist.pointAt(curr_npc->getBodyPosition());
                                npclist.setCurrent(*curr_npc);
                                map->setNPCList(npclist);
                                screen.write_textbox_npc_weapon(*curr_npc, player.getWeapon());
                            }
                        }

                    }
                }
            }

        } // if (_kbhit())

        /*** Controlla se il giocatore si trova su un terreno solido ***/
        if (!map->isSolidAt(Position(player.getBodyPosition().getX(), player.getBodyPosition().getY()+1))) {
            player.setOnTerrain(false);
        }

        /*** Gestione salto ***/
        if (player.canJump()) { 
            // Interrompe il salto se raggiunge il soffitto o un doppio blocco solido
            if (player.getHeadPosition().getY() <= 1 || 
               (map->isSolidAt(Position(player.getHeadPosition().getX(), player.getHeadPosition().getY()-1)) && map->isSolidAt(Position(player.getHeadPosition().getX(), player.getHeadPosition().getY()-2))) ) {
                player.stopJump();
            }
            else {
                screen.resetTerrain(map, player.getBodyPosition());
                screen.resetTerrain(map, player.getHeadPosition());

                player.jump();

                screen.write_entity(map, player);
            }
        }
        /*** Gestione caduta ***/
        else if (player.canFall()) {
            screen.resetTerrain(map, player.getBodyPosition());
            screen.resetTerrain(map, player.getHeadPosition());

            player.fall();

            screen.write_entity(map, player);
        }
        /*** Gestione arrivo a terra ***/
        else if (map->isSolidAt(Position(player.getBodyPosition().getX(), player.getBodyPosition().getY()+1))) {
            player.setOnTerrain(true);
        }
        /*** Gestione animazione arma ***/
        if (player.endWeaponDisplay()) {
            screen.resetTerrain(map, player.getFrontPosition());
            player.setCanMove(true);
        }

        /*** Gestione tempo di ricarica ***/
        if (player.hasReloadFinished()) {
            screen.write_ammobox(player.getWeapon().getCurrAmmo());
        }

        /*** Gestione velocità d'attacco ***/
        if (player.hasShootDelayFinished()) {
            screen.write_ammobox(player.getWeapon().getCurrAmmo());
        }

        /*** Gestione bonus ***/
        if (curr_bonus == NULL) {
            BonusList bonuslist = map->getBonusList();
            if (bonuslist.pointAt(player.getBodyPosition())) {
                Bonus bonus = bonuslist.getCurrent();
            
                player.incPoints(bonus.getPoints());
                screen.write_points(player.getPoints());

                if (bonus.getType() == BONUS_TYPE_HP) {
                    player.heal(bonus.getBonus());
                    screen.write_hp(player.getHealth());
                    bonuslist.deleteCurrent();
                }
                else if (bonus.getType() == BONUS_TYPE_MONEY) {
                    player.incMoney(bonus.getBonus());
                    screen.write_money(player.getMoney());
                    bonuslist.deleteCurrent();
                }
                else if (bonus.getType() == BONUS_TYPE_WEAPON) {
                    screen.write_textbox_weaponbonus(bonus.getWeapon(), player.getWeapon());
                    delete curr_bonus;
                    curr_bonus = new Bonus();
                    *curr_bonus = bonus;
                }
            }
            map->setBonusList(bonuslist);
        }

        player.incCounters();
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

                if (bullet.isHostile()) {
                    if (player.getBodyPosition().equals(bullet.getPosition()) || player.getHeadPosition().equals(bullet.getPosition())) {
                        player.take_damage(bullet.hit());
                        screen.write_hp(player.getHealth());
                    }
                }
                else {
                    if (enemylist.pointAt(bullet.getPosition())) {
                        Enemy hit_enemy = enemylist.getCurrent();
                        hit_enemy.take_damage(bullet.hit());

                        if (hit_enemy.isDead()) {
                            screen.resetTerrain(map, hit_enemy.getHeadPosition());
                            screen.resetTerrain(map, hit_enemy.getBodyPosition());
                            player.incPoints(hit_enemy.getPoints());
                            screen.write_points(player.getPoints());
                            player.incMoney(hit_enemy.getMoney());
                            screen.write_money(player.getMoney());
                        }
                        enemylist.updateCurrent(hit_enemy);
                        map->setEnemyList(enemylist);
                    }
                }
                
                if (!enemylist.existsAt(bullet.getPosition()) &&
                    !player.getBodyPosition().equals(bullet.getPosition()) &&
                    !player.getHeadPosition().equals(bullet.getPosition())) {
                    screen.resetTerrain(map, bullet.getPosition());
                }
                if (bullet.travel()) {
                    if (!enemylist.existsAt(bullet.getPosition()) && 
                        !player.getBodyPosition().equals(bullet.getPosition()) && 
                        !player.getHeadPosition().equals(bullet.getPosition())) {
                        screen.write_bullet(map, bullet);
                    }
                }
            }

            bullet.incRefresh();

            bulletlist.updateCurrent(bullet);
            map->setBulletList(bulletlist);
        }
        /*  FINE GESTIONE PROIETTILI   
        --------------------------- */



        /* -------------------------
           INIZIO GESTIONE NEMICI   
        ------------------------- */
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
                    screen.write_entity(map, enemy);
                }
                else if (action == ACTION_GO_LEFT) {
                    screen.resetTerrain(map, enemy.getBodyPosition());
                    screen.resetTerrain(map, enemy.getHeadPosition()); 
                    if (player.getCanMove() && (!map->isSolidAt(enemy.getFrontPosition()) && player.getBodyPosition().getX() != enemy.getFrontPosition().getX() || (enemy.getDirection() == DIRECTION_RIGHT))) {
                        enemy.goLeft();
                    }
                    screen.write_entity(map, enemy);
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
                    if (enemy.isOnTerrain()) {
                        if (enemy.canAttack()) {
                            map->addBullet(enemy.attack());

                            screen.write_at(map, enemy.getWeapon().getTexture(enemy.getDirection()), enemy.getFrontPosition());
                        }
                        else {
                            if (enemy.canReload() && !enemy.getWeapon().hasAmmo()) {
                                enemy.reload();
                            }
                        }
                    }
                }
            }

            /*** Controlla se il nemico si trova su un terreno solido ***/
            if (!map->isSolidAt(Position(enemy.getBodyPosition().getX(), enemy.getBodyPosition().getY()+1))) {
                enemy.setOnTerrain(false);
            }

            /*** Gestione salto ***/
            if (enemy.canJump()) {
                // Interrompe il salto se raggiunge il soffitto, un doppio blocco solido o il giocatore
                if (enemy.getHeadPosition().getY() <= 1 || 
                     (map->isSolidAt(Position(enemy.getHeadPosition().getX(), enemy.getHeadPosition().getY()-1)) && 
                     map->isSolidAt(Position(enemy.getHeadPosition().getX(), enemy.getHeadPosition().getY()-2))) || 
                     player.getBodyPosition().equals(Position(enemy.getHeadPosition().getX(), enemy.getHeadPosition().getY()-2))
                    ) {
                    enemy.stopJump();
                }
                else {
                    screen.resetTerrain(map, enemy.getBodyPosition());
                    screen.resetTerrain(map, enemy.getHeadPosition());

                    enemy.jump();

                    screen.write_entity(map, enemy);
                }
            }
            /*** Gestione caduta ***/
            else if (enemy.canFall() && !player.getHeadPosition().equals(Position(enemy.getHeadPosition()))) {
                screen.resetTerrain(map, enemy.getBodyPosition());
                screen.resetTerrain(map, enemy.getHeadPosition());

                enemy.fall();

                screen.write_entity(map, enemy);
            }
            /*** Gestione arrivo a terra ***/
            else if (map->isSolidAt(Position(enemy.getBodyPosition().getX(), enemy.getBodyPosition().getY()+1)) || 
                    player.getHeadPosition().equals(Position(enemy.getBodyPosition().getX(), enemy.getBodyPosition().getY()+1))) {
                enemy.setOnTerrain(true);
            }
            
            /*** Gestione collisione con proiettili ***/
            bulletlist = map->getBulletList();
            if (bulletlist.pointAt(enemy.getHeadPosition(), enemy.getBodyPosition())) {
                Bullet hit_bullet = bulletlist.getCurrent();

                if (!hit_bullet.isHostile()) {
                    enemy.take_damage(hit_bullet.hit());
                    if (enemy.isDead()) {
                        screen.resetTerrain(map, enemy.getHeadPosition());
                        screen.resetTerrain(map, enemy.getBodyPosition());

                        player.incPoints(enemy.getPoints());
                        screen.write_points(player.getPoints());

                        //player.incMoney(enemy.getMoney());
                        //screen.write_money(player.getMoney());

                        screen.resetTerrain(
                            map, 
                            map->addBonus(Bonus(Pixel(MONEY_SYMBOL, FG_DARKYELLOW, BACKGROUND_DEFAULT, false), enemy.getBodyPosition(), 0, enemy.getMoney(), 0))
                        );

                    }

                    bulletlist.updateCurrent(hit_bullet);
                    map->setBulletList(bulletlist);
                }
            }

            /*** Gestione animazione arma ***/
            if (enemy.endWeaponDisplay()) {
                screen.resetTerrain(map, enemy.getFrontPosition());
                enemy.setCanMove(true);
            }

            /*** Gestione tempo di ricarica ***/
            enemy.hasReloadFinished();

            /*** Gestione velocità d'attacco ***/
            enemy.hasShootDelayFinished();

            enemy.incCounters();

            enemylist.updateCurrent(enemy);
            map->setEnemyList(enemylist);
        }
        /* FINE GESTIONE NEMICI
        ----------------------- */



        /* ----------------------
           INIZIO GESTIONE NPC   
        ---------------------- */

        NPCList npcList = map->getNPCList();

        // Visualizza gli NPC sulla mappa
        if (showNPCs) {
            npcList.initIter();
            while(!npcList.isNull()) {
                screen.write_entity(map, npcList.getCurrent());
                npcList.goNext();
            }

            showNPCs = false;
        }

        // Gestisce l'interazione con gli NPC
        if (curr_npc == NULL) {
            if (npcList.inPlayerRange(player.getBodyPosition())) {
                NPC npc = npcList.getCurrent();
                char name[STRING_LEN];
                npc.getName(name);

                if (npc.getType() == NPC_HOSPITAL) {
                    screen.write_textbox_npc_hp(npc, player.getMissingHp());
                }
                else if (npc.getType() == NPC_WEAPONSHOP) {
                    screen.write_textbox_npc_weapon(npc, player.getWeapon());
                }

                delete curr_npc;
                curr_npc = new NPC();
                *curr_npc = npc;
            }
        }

        /* FINE GESTIONE NPC
        -------------------- */

        
        screen.incWeaponboxRotateCounter();
    } // while (true)
}
