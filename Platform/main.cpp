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
#include "EntityGenerator.h"
using namespace std;

const char KEY_SPACEBAR = 32;

int main() {
    srand(time(0));

    Screen screen = Screen();
    Map *map = new Map();

    WeaponContainer *weapon_container = new WeaponContainer();
    weapon_container->initForPlayer();
    Player player = Player(MAX_LIFE, 
        Pixel(char(17), PLAYER_HEAD_COLOR_FG, BACKGROUND_DEFAULT, true),
        Pixel(char(16), PLAYER_HEAD_COLOR_FG, BACKGROUND_DEFAULT, true),
        Pixel(char(219), PLAYER_BODY_COLOR_FG, BACKGROUND_DEFAULT, true), 
        map->getLeftPosition(), weapon_container->getRandomTier1()
    );
    delete weapon_container;

    Bonus *curr_bonus = NULL;
    NPC *curr_npc = NULL;
    bool hasMoved;
    bool showNPCs = true;
    bool print_boss_hp = true;
    
    screen.init();
    screen.write_game_area(map);
    screen.write_entity(map, player);
    screen.write_weaponInfo(player.getWeapon());
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
                    screen.remove_entity(map, player);
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
            else if (ch == 'd' || ch == 'D') {
                hasMoved = true;
                if (player.getCanMove() && (!map->isSolidAt(player.getFrontPosition()) || (player.getDirection() == DIRECTION_LEFT))) {
                    screen.remove_entity(map, player);
                    player.goRight();
                    screen.write_entity(map, player);

                    if (player.getBodyPosition().getX() >= GAME_WIDTH) {
                        map = map->gotoNext(player.getBodyPosition());
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
                    screen.remove_entity(map, player);
                    player.fall();
                }
            }
            else if (ch == KEY_SPACEBAR) {
                if (player.isOnTerrain()) {
                    if (player.canAttack()) {
                        if (player.getWeapon().getType() == WEAPON_MELEE) {
                            Bullet melee_bullet = player.attack();
                            int range = player.getWeapon().getRange();
                            int i = 0;
                            if (player.getDirection() == DIRECTION_LEFT) {
                                while (i < range && melee_bullet.getPosition().getX()-i >= 1) {
                                    Bullet temp = melee_bullet;
                                    temp.setRange(1);
                                    temp.setPosition(Position(melee_bullet.getPosition().getX()-i, melee_bullet.getPosition().getY()));
                                    map->addBullet(temp);
                                    screen.write_bullet(map, player, temp);
                                    i++;
                                }
                            }
                            else {
                                while (i < range && melee_bullet.getPosition().getX()+i <= GAME_WIDTH) {
                                    Bullet temp = melee_bullet;
                                    temp.setRange(1);
                                    temp.setPosition(Position(melee_bullet.getPosition().getX()+i, melee_bullet.getPosition().getY()));
                                    map->addBullet(temp);
                                    screen.write_bullet(map, player, temp);
                                    i++;
                                }
                            }
                            player.reload();
                            screen.write_ammobox(player.getWeapon().getCurrAmmo());
                        }
                        else {
                            Bullet bullet = player.attack();
                            map->addBullet(bullet);
                            screen.write_bullet(map, player, bullet);
                        }

                        screen.write_at(map, player.getWeapon().getTexture(player.getDirection()), player.getFrontPosition());
                    }
                    else {
                        if (player.canReload() && !player.getWeapon().hasAmmo()) {
                            player.reload();
                        }
                    }
                }
                if(player.getWeapon().hasAmmo())
                screen.write_ammobox(player.getWeapon().getCurrAmmo());
            }
            else if (ch == 'r' || ch == 'R') {
                player.reload();
                screen.write_ammobox(player.getWeapon().getCurrAmmo());
            }

            // Se hasMoved è true, il giocatore si è mosso dall'eventuale bonus/NPC su cui si trovava
            if (hasMoved) {
                if (!map->isBossFight()) {
                    curr_bonus = NULL;
                    curr_npc = NULL;
                    screen.clear_textbox();
                }
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
                    screen.write_weaponInfo(player.getWeapon());
                }
            }

            // Gestione NPC
            if (curr_npc != NULL) {
                if (curr_npc->getType() == NPC_HOSPITAL) {
                    if (ch == 'e' || ch == 'E') {
                        if (player.getHealth() != MAX_LIFE) {
                            if (player.getMoney() >= curr_npc->getPriceHP()) {
                                player.decMoney(curr_npc->getPriceHP());
                                screen.write_money(player.getMoney());
                                player.heal(1);
                                screen.write_hp(player.getHealth());
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
                                screen.write_weaponInfo(player.getWeapon());

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
                (map->isSolidAt(Position(player.getHeadPosition().getX(), player.getHeadPosition().getY()-1)) && 
                map->isSolidAt(Position(player.getHeadPosition().getX(), player.getHeadPosition().getY()-2))) ) {
                player.stopJump();
            }
            else {
                screen.remove_entity(map, player);
                player.jump();
                screen.write_entity(map, player);
            }
        }
        /*** Gestione caduta ***/
        else if (player.canFall()) {
            screen.remove_entity(map, player);
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

        /*** Gestione lava ***/
        if (player.canMapEvents() && map->isLava(player.getBelowPosition())) {
            player.take_damage(1);
            screen.write_hp(player.getHealth());
        }

        player.incCounters();
        /* FINE GESTIONE GIOCATORE   
        -------------------------- */



    /* -----------------------------
        INIZIO GESTIONE PROIETTILI   
    ----------------------------- */
        BulletList bulletlist = map->getBulletList();
        bulletlist.initIter();

        while (!bulletlist.isNull()) {
            EnemyList enemylist = map->getEnemyList();
            Bullet bullet = bulletlist.getCurrent();

            if (bullet.canRefresh()) {
                if (bullet.isHostile()) {
                    if (player.existsAt(bullet.getPosition())) {
                        player.take_damage(bullet.hit());
                        screen.write_hp(player.getHealth());
                    }
                }
                else {
                    if (enemylist.pointAt(bullet.getPosition())) {
                        Enemy hit_enemy = enemylist.getCurrent();
                        hit_enemy.take_damage(bullet.hit());

                        if (hit_enemy.isDead()) {
                            screen.remove_entity(map, hit_enemy);
                            player.incPoints(hit_enemy.getPoints());
                            screen.write_points(player.getPoints());

                            if (hit_enemy.getMoney() > 0) {
                                screen.resetTerrain(
                                    map,
                                    map->addBonus(Bonus(Pixel(MONEY_SYMBOL, FG_DARKYELLOW, BACKGROUND_DEFAULT, false), hit_enemy.getBodyPosition(), 0, hit_enemy.getMoney(), 0))
                                );
                            }
                        }
                        enemylist.updateCurrent(hit_enemy);
                        map->setEnemyList(enemylist);
                    }
                    else if (map->isBossFight()) {
                        Boss *boss = map->getBoss();
                        if (boss->existsAt(bullet.getPosition())) {
                            boss->take_damage(bullet.hit());
                            screen.write_write_boss_hp(*boss);

                            if (boss->isDead()) {
                                screen.remove_boss(map, *boss);
                                player.incPoints(boss->getPoints());
                                screen.write_points(player.getPoints());

                                screen.resetTerrain(
                                    map,
                                    map->addBonus(Bonus(Pixel(MONEY_SYMBOL, FG_DARKYELLOW, BACKGROUND_DEFAULT, false), boss->getBodyPosition(), 0, boss->getMoney(), 0))
                                );
                                delete boss;
                                map->setBoss(NULL);
                            }
                            else {
                                map->setBoss(boss);
                            }
                        }
                    }
                }
                
                bool boss_exists = false;
                if (map->isBossFight()) {
                    boss_exists = map->getBoss()->existsAt(bullet.getPosition());
                }

                if (!enemylist.existsAt(bullet.getPosition()) && !player.existsAt(bullet.getPosition()) && !boss_exists) {
                    screen.resetTerrain(map, bullet.getPosition());
                }
                if (bullet.travel()) {
                    screen.write_bullet(map, player, bullet);
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
        EnemyList enemylist = map->getEnemyList();
        enemylist.initIter();

        while (!enemylist.isNull()) {
            Enemy enemy = enemylist.getCurrent();

            if (enemy.canRefresh()) {
                enemy.search4Player(player);

                int action = enemy.getAction(map, player);

                if (action == ACTION_GO_RIGHT) {
                    if (enemy.getCanMove() && (!map->isSolidAt(enemy.getFrontPosition()) && !player.existsAt(enemy.getFrontPosition()) || (enemy.getDirection() == DIRECTION_LEFT))) {
                        screen.remove_entity(map, enemy);
                        enemy.goRight();
                        screen.write_entity(map, enemy);
                    }
                }
                else if (action == ACTION_GO_LEFT) {
                    if (enemy.getCanMove() && (!map->isSolidAt(enemy.getFrontPosition()) && !player.existsAt(enemy.getFrontPosition()) || (enemy.getDirection() == DIRECTION_RIGHT))) {
                        screen.remove_entity(map, enemy);
                        enemy.goLeft();
                        screen.write_entity(map, enemy);
                    }
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
                        screen.remove_entity(map, enemy);
                        enemy.fall();
                    }
                }
                else if (action == ACTION_ATTACK) {
                    if (enemy.isOnTerrain()) {
                        if (enemy.canAttack()) {
                            if (enemy.getWeapon().getType() == WEAPON_MELEE) {
                                Bullet melee_bullet = enemy.attack();
                                int range = enemy.getWeapon().getRange();
                                int i = 0;
                                if (enemy.getDirection() == DIRECTION_LEFT) {
                                    while (i < range && melee_bullet.getPosition().getX()-i >= 1) {
                                        Bullet temp = melee_bullet;
                                        temp.setRange(1);
                                        temp.setPosition(Position(melee_bullet.getPosition().getX()-i, melee_bullet.getPosition().getY()));
                                        map->addBullet(temp);
                                        screen.write_bullet(map, player, temp);
                                        i++;
                                    }
                                }
                                else {
                                    while (i < range && melee_bullet.getPosition().getX()+i <= GAME_WIDTH) {
                                        Bullet temp = melee_bullet;
                                        temp.setRange(1);
                                        temp.setPosition(Position(melee_bullet.getPosition().getX()+i, melee_bullet.getPosition().getY()));
                                        map->addBullet(temp);
                                        screen.write_bullet(map, player, temp);
                                        i++;
                                    }
                                }
                                enemy.reload();
                            }
                            else {
                                Bullet bullet = enemy.attack();
                                map->addBullet(bullet);
                                screen.write_bullet(map, player, bullet);
                            }
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
                    player.existsAt(Position(enemy.getHeadPosition().getX(), enemy.getHeadPosition().getY()-2))) {
                    enemy.stopJump();
                }
                else {
                    screen.remove_entity(map, enemy);
                    enemy.jump();
                    screen.write_entity(map, enemy);
                }
            }
            /*** Gestione caduta ***/
            else if (enemy.canFall()) {
                screen.remove_entity(map, enemy);
                enemy.fall();
                screen.write_entity(map, enemy);
            }
            /*** Gestione arrivo a terra ***/
            else if (map->isSolidAt(Position(enemy.getBodyPosition().getX(), enemy.getBodyPosition().getY()+1)) || 
                    player.existsAt(Position(enemy.getBodyPosition().getX(), enemy.getBodyPosition().getY()+1))) {
                enemy.setOnTerrain(true);
            }
            
            /*** Gestione collisione con proiettili ***/
            bulletlist = map->getBulletList();
            if (bulletlist.pointAt(enemy.getHeadPosition(), enemy.getBodyPosition())) {
                Bullet hit_bullet = bulletlist.getCurrent();

                if (!hit_bullet.isHostile()) {
                    enemy.take_damage(hit_bullet.hit());
                    if (enemy.isDead()) {
                        screen.remove_entity(map, enemy);

                        player.incPoints(enemy.getPoints());
                        screen.write_points(player.getPoints());

                        if (enemy.getMoney() > 0) {
                            screen.resetTerrain(
                                map, 
                                map->addBonus(Bonus(Pixel(MONEY_SYMBOL, FG_DARKYELLOW, BACKGROUND_DEFAULT, false), enemy.getBodyPosition(), 0, enemy.getMoney(), 0))
                            );
                        }
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

            /*** Gestione lava ***/
            if (enemy.canMapEvents() && map->isLava(enemy.getBelowPosition())) {
                enemy.take_damage(1);
                if (enemy.isDead()) {
                    screen.remove_entity(map, enemy);
                }
            }

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


        /* -----------------------
           INIZIO GESTIONE BOSS
        ----------------------- */

        if (map->isBossFight()) {
            Boss *boss = map->getBoss();

            if (print_boss_hp) {
                screen.write_write_boss_hp(*boss);
                print_boss_hp = false;
            }

            if (boss->canRefresh()) {
                boss->search4Player(player);

                int action = boss->getAction(map, player);

                if (action == ACTION_GO_RIGHT) {
                    if (boss->getCanMove() && (!map->isSolidAt(boss->getFrontPosition()) && 
                        (!player.existsAt(boss->getFrontPosition()) && !player.existsAt(boss->getHeadPosition()) && !player.existsAt(boss->getBody2Position())) ||
                        (boss->getDirection() == DIRECTION_LEFT))) {
                        screen.remove_boss(map, *boss);
                        boss->goRight();
                        screen.write_boss(map, *boss);
                    }
                }
                else if (action == ACTION_GO_LEFT) {
                    if (boss->getCanMove() && (!map->isSolidAt(boss->getFrontPosition()) && 
                        (!player.existsAt(boss->getFrontPosition()) && !player.existsAt(boss->getHeadPosition()) && !player.existsAt(boss->getBody2Position())) ||
                        (boss->getDirection() == DIRECTION_RIGHT))) {
                        screen.remove_boss(map, *boss);
                        boss->goLeft();
                        screen.write_boss(map, *boss);
                    }
                }
                else if (action == ACTION_JUMP) {
                    if (boss->isOnTerrain()) {
                        boss->initJump();
                    }
                }
                else if (action == ACTION_FALL) {
                    if (map->isSolidAt(Position(boss->getBodyPosition().getX(), boss->getBodyPosition().getY()+1)) &&
                        !map->isSolidAt(Position(boss->getBodyPosition().getX(), boss->getBodyPosition().getY()+2)) &&
                        !map->isSolidAt(Position(boss->getBodyPosition().getX(), boss->getBodyPosition().getY()+3))) {

                        boss->setOnTerrain(false);
                        screen.remove_boss(map, *boss);
                        boss->fall();
                    }
                }
                else if (action == ACTION_ATTACK) {
                    if (boss->getType() == BOSS_TYPE1) {
                        EnemyList enemylist = map->getEnemyList();

                        for (int i=0; i<boss->getAbilityNum(); i++) {
                            int x = boss->getBodyPosition().getX() + i*3;
                            if (x > GAME_WIDTH) {
                                x = boss->getBodyPosition().getX() - i*3;
                            }
                            int y = boss->getBodyPosition().getY() + 3;

                            Enemy new_enemy = createEnemy(map->getDifficulty());
                            new_enemy.setMoney(0);
                            new_enemy.setPosition(Position(x, y));
                            enemylist.add(new_enemy);
                        }

                        map->setEnemyList(enemylist);
                    }
                }
            }

            if (map->getEnemyList().size() == 0 && boss->getPhase() == 1) {
                boss->nextPhase();
            }

            /*** Controlla se il nemico si trova su un terreno solido ***/
            if (!map->isSolidAt(Position(boss->getBodyPosition().getX(), boss->getBodyPosition().getY()+1))) {
                boss->setOnTerrain(false);
            }

            /*** Gestione salto ***/
            if (boss->canJump()) {
                // Interrompe il salto se raggiunge il soffitto, un doppio blocco solido o il giocatore
                if (boss->getHeadPosition().getY() <= 1 ||
                    (map->isSolidAt(Position(boss->getHeadPosition().getX(), boss->getHeadPosition().getY()-1)) &&
                    map->isSolidAt(Position(boss->getHeadPosition().getX(), boss->getHeadPosition().getY()-2))) ||
                    player.existsAt(Position(boss->getHeadPosition().getX(), boss->getHeadPosition().getY()-2))
                    ) {
                    boss->stopJump();
                }
                else {
                    screen.remove_boss(map, *boss);
                    boss->jump();
                    screen.write_boss(map, *boss);
                }
            }
            /*** Gestione caduta ***/
            else if (boss->canFall()) {
                screen.remove_boss(map, *boss);
                boss->fall();
                screen.write_boss(map, *boss);
            }
            /*** Gestione arrivo a terra ***/
            else if (map->isSolidAt(Position(boss->getBodyPosition().getX(), boss->getBodyPosition().getY()+1)) ||
                player.existsAt(Position(boss->getBodyPosition().getX(), boss->getBodyPosition().getY()+1))) {
                boss->setOnTerrain(true);
            }

            /*** Gestione collisione con proiettili ***/
            bulletlist = map->getBulletList();
            if (bulletlist.pointAt(boss->getHeadPosition(), boss->getBodyPosition(), boss->getBody2Position())) {
                Bullet hit_bullet = bulletlist.getCurrent();

                if (!hit_bullet.isHostile()) {
                    boss->take_damage(hit_bullet.hit());
                    screen.write_write_boss_hp(*boss);
                    if (boss->isDead()) {
                        screen.remove_boss(map, *boss);

                        player.incPoints(boss->getPoints());
                        screen.write_points(player.getPoints());

                        screen.resetTerrain(
                            map,
                            map->addBonus(Bonus(Pixel(MONEY_SYMBOL, FG_DARKYELLOW, BACKGROUND_DEFAULT, false), boss->getBodyPosition(), 0, boss->getMoney(), 0))
                        );

                        map->endBossFight();
                        screen.write_terrain(map);
                        screen.write_entity(map, player);
                        screen.write_bonuses(map, map->getBonusList());
                        screen.clear_textbox();
                        print_boss_hp = true;
                    }

                    bulletlist.updateCurrent(hit_bullet);
                    map->setBulletList(bulletlist);
                }
            }

            /*** Gestione animazione arma ***/
            if (boss->endWeaponDisplay()) {
                screen.resetTerrain(map, boss->getFrontPosition());
                boss->setCanMove(true);
            }

            /*** Gestione tempo di ricarica ***/
            boss->hasReloadFinished();

            /*** Gestione velocità d'attacco ***/
            boss->hasShootDelayFinished();

            boss->incCounters();
            if (boss->isDead()) {
                delete boss;
                map->setBoss(NULL);
            }
            else {
                map->setBoss(boss);
            }
        }

        
        screen.incWeaponboxRotateCounter();
    } // while (true)
}
