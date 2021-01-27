#include <conio.h>
#include <cstdlib>
#include <ctime>
#include "Screen.hpp"
#include "Map.hpp"
#include "Player.hpp"
#include "settings.h"
#include "EntityGenerator.h"
using namespace std;

const char KEY_SPACEBAR = 32;

void moveLeft(Map *map, Screen screen, Entity &entity);
void moveRight(Map *map, Screen screen, Entity &entity);
void jump(Map *map, Screen screen, Entity &entity);
void fall(Map *map, Screen screen, Entity &entity);

void take_damage(Screen screen, Player &player, int damage);
void heal(Screen screen, Player &player, int heal);

void spend_money(Screen screen, Player &player, int money);
void get_money(Screen screen, Player &player, int money);

void increase_points(Screen screen, Player &player, int points);

void ranged_attack(Map *map, Screen screen, Player player, ArmedEntity &entity, bool hostile);
void melee_attack(Map *map, Screen screen, Player player, ArmedEntity &entity, bool hostile);

int main() {
    srand(time(0));

    Screen screen = Screen();
    Map *map = new Map();

    Player player = Player(MAX_LIFE, 
        Pixel(PLAYER_HEAD_LEFT, PLAYER_HEAD_COLOR_FG, BACKGROUND_DEFAULT, true),
        Pixel(PLAYER_HEAD_RIGHT, PLAYER_HEAD_COLOR_FG, BACKGROUND_DEFAULT, true),
        Pixel(PLAYER_BODY, PLAYER_BODY_COLOR_FG, BACKGROUND_DEFAULT, true),
        map->getLeftPosition(), getRandomTier1Player()
    );

    Bonus *curr_bonus = NULL;
    NPC *curr_npc = NULL;
    bool hasMoved;
    bool showNPCs = true;
    bool print_boss_hp = true;

    EnemyList enemylist;
    BulletList bulletlist;
    BonusList bonuslist;
    NPCList npclist;

    screen.init();
    screen.write_game_area(map);
    screen.write_entity(map, player);
    screen.write_weaponInfo(player.getWeapon());
    screen.write_money(player.getMoney());
    screen.write_hp(player.getHealth());

    while (!player.isDead()) {

        // Gestione rotazione testo nome arma
        if (screen.canRotateWeaponbox()) {
            screen.rotate_weaponbox();
        }

        /* ----------------------------
           INIZIO GESTIONE GIOCATORE   
        ---------------------------- */
        hasMoved = false;

        if (_kbhit()) {
            char ch = _getch();

            // MOVIMENTO A SINISTRA
            if (ch == 'a' || ch == 'A') {
                if ((!map->isSolidAt(player.getBodyFrontPosition()) && !map->isSolidAt(player.getHeadFrontPosition()) || player.getDirection() == DIRECTION_RIGHT)) {
                    hasMoved = true;
                    moveLeft(map, screen, player);

                    if (player.getBodyPosition().getX() <= 0) { // Accesso area precedente
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
            // MOVIMENTO A DESTRA
            else if (ch == 'd' || ch == 'D') {
                if ((!map->isSolidAt(player.getBodyFrontPosition()) && !map->isSolidAt(player.getHeadFrontPosition()) || player.getDirection() == DIRECTION_LEFT)) {
                    hasMoved = true;
                    moveRight(map, screen, player);

                    if (player.getBodyPosition().getX() >= GAME_WIDTH-1) { // Accesso area successiva
                        map = map->gotoNext(player.getBodyPosition());
                        screen.write_game_area(map);
                        player.setPosition(map->getLeftPosition());
                        screen.write_entity(map, player);
                        showNPCs = true;
                    }
                }
            }
            // SALTO
            else if (ch == 'w' || ch == 'W' ) {
                if (player.isOnTerrain()) {
                    hasMoved = true;
                    player.initJump();
                }
            }
            // CADUTA
            else if (ch == 's' || ch == 'S') {
                if (map->isSolidAt(Position(player.getBodyPosition().getX(), player.getBodyPosition().getY()+1)) &&
                    !map->isSolidAt(Position(player.getBodyPosition().getX(), player.getBodyPosition().getY()+2)) &&
                    !map->isSolidAt(Position(player.getBodyPosition().getX(), player.getBodyPosition().getY()+3)) ) {
                    
                    hasMoved = true;
                    player.setOnTerrain(false);
                    fall(map, screen, player);
                }
            }
            // ATTACCO
            else if (ch == KEY_SPACEBAR) {
                if (player.isOnTerrain()) {
                    if (player.canAttack()) {
                        if (player.getWeapon().getType() == WEAPON_MELEE) {
                            melee_attack(map, screen, player, player, false);
                        }
                        else {
                            ranged_attack(map, screen, player, player, false);
                        }

                        screen.write_weapon(map, player);
                        screen.write_ammobox(player.getWeapon().getCurrAmmo());
                    }
                    else {
                        if (player.canReload() && !player.getWeapon().hasAmmo()) {
                            player.reload();
                            screen.write_ammobox(player.getWeapon().getCurrAmmo());
                        }
                    }
                }
            }
            // RICARICA
            else if (ch == 'r' || ch == 'R') {
                player.reload();
                screen.write_ammobox(player.getWeapon().getCurrAmmo());
            }

            // Se hasMoved è true, il giocatore si è mosso dall'eventuale bonus/NPC su cui si trovava
            if (hasMoved) {
                if (!map->isBossFight()) {
                    delete curr_bonus;
                    delete curr_npc;
                    screen.clear_textbox();
                }
            }

            // Gestione bonus di tipo arma
            if (curr_bonus != NULL) {
                if (ch == 'e' || ch == 'E') { // Scambia la tua arma con quella del bonus
                    Weapon old = player.getWeapon();
                    player.setWeapon(curr_bonus->getWeapon());

                    bonuslist = map->getBonusList();
                    bonuslist.pointAt(curr_bonus->getBodyPosition());
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
                    if (ch == 'e' || ch == 'E') { // Cura 1
                        if (player.getHealth() != MAX_LIFE) {
                            if (player.getMoney() >= curr_npc->getPriceHP()) {
                                spend_money(screen, player, curr_npc->getPriceHP());
                                heal(screen, player, 1);
                                screen.write_textbox_npc_hp(*curr_npc, player.getMissingHp());
                            }
                        }
                    }
                    else if (ch == 'q' || ch == 'Q') { // Cura tutto
                        if (player.getMoney() >= curr_npc->getPriceHP()*player.getMissingHp()) {
                            spend_money(screen, player, curr_npc->getPriceHP()*player.getMissingHp());
                            heal(screen, player, player.getMissingHp());
                            screen.write_textbox_npc_hp(*curr_npc, player.getMissingHp());
                        }
                    }
                }
                else if (curr_npc->getType() == NPC_WEAPONSHOP) {
                    if (curr_npc->getWeaponNumber() > 0) {
                        if (ch == 'o' || ch == 'O') { // Navigazione sx
                            curr_npc->setCurrSelected(curr_npc->getCurrSelected() - 1);
                            screen.write_textbox_npc_weapon(*curr_npc, player.getWeapon());
                        }
                        else if (ch == 'p' || ch == 'P') { // Navigazione dx
                            curr_npc->setCurrSelected(curr_npc->getCurrSelected() + 1);
                            screen.write_textbox_npc_weapon(*curr_npc, player.getWeapon());
                        }
                        else if (ch == 'e' || ch == 'E') { // Compra
                            if (player.getMoney() >= curr_npc->getCurrWeaponPrice()) {
                                spend_money(screen, player, curr_npc->getCurrWeaponPrice());
                                player.setWeapon(curr_npc->getCurrWeapon());
                                screen.write_weaponInfo(player.getWeapon());

                                curr_npc->deleteCurrentWeapon();
                                npclist = map->getNPCList();
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
            if (player.getHeadPosition().getY() <= 0 || 
                (map->isSolidAt(Position(player.getHeadPosition().getX(), player.getHeadPosition().getY()-1)) && 
                map->isSolidAt(Position(player.getHeadPosition().getX(), player.getHeadPosition().getY()-2))) ) {
                player.stopJump();
            }
            else {
                jump(map, screen, player);
            }
        }
        /*** Gestione caduta ***/
        else if (player.canFall()) {
            fall(map, screen, player);
        }
        /*** Gestione arrivo a terra ***/
        else if (map->isSolidAt(player.getBelowPosition())) {
            player.setOnTerrain(true);
        }

        /*** Gestione tempo di ricarica ***/
        if (player.hasReloadFinished()) {
            screen.write_ammobox(player.getWeapon().getCurrAmmo());
        }

        /*** Gestione velocità d'attacco ***/
        if (player.hasShootDelayFinished()) {
            //screen.write_ammobox(player.getWeapon().getCurrAmmo());
        }

        /*** Gestione collisione con proiettili ***/
        bulletlist = map->getBulletList();
        if (bulletlist.pointAt(player.getHeadPosition(), player.getBodyPosition())) {
            Bullet hit_bullet = bulletlist.getCurrent();

            if (hit_bullet.isHostile()) {
                take_damage(screen, player, hit_bullet.hit());

                bulletlist.updateCurrent(hit_bullet);
                map->setBulletList(bulletlist);
            }
        }

        /*** Gestione bonus ***/
        if (curr_bonus == NULL) {
            bonuslist = map->getBonusList();
            if (bonuslist.pointAt(player.getBodyPosition())) {
                Bonus bonus = bonuslist.getCurrent();
            
                increase_points(screen, player, bonus.getPoints());

                if (bonus.getType() == BONUS_TYPE_HP) {
                    heal(screen, player, bonus.getBonus());
                    bonuslist.deleteCurrent();
                }
                else if (bonus.getType() == BONUS_TYPE_MONEY) {
                    get_money(screen, player, bonus.getBonus());
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
        if (map->isLava(player.getBelowPosition())) {
            if (player.isMapEvent()) { // Gestisce danni se il giocatore è già sulla lava
                if (player.canMapEvents()) {
                    take_damage(screen, player, LAVA_DAMAGE);
                }
            }
            else { // Gestisce danni se il giocatore va per la prima volta sulla lava
                take_damage(screen, player, LAVA_DAMAGE);
                player.startMapEvent();
            }
        }
        else { // Gestione uscita dalla lava
            if (player.isMapEvent()) {
                player.stopMapEvent();
            }
        }

        player.incCounters();
        /* FINE GESTIONE GIOCATORE   
        -------------------------- */


        /* -------------------------
           INIZIO GESTIONE NEMICI   
        ------------------------- */
        enemylist = map->getEnemyList();
        enemylist.initIter();

        while (!enemylist.isNull()) {
            Enemy enemy = enemylist.getCurrent();

            if (enemy.canRefresh()) {

                // Cerca il giocatore e decide l'azione successiva
                enemy.searchForPlayer(player);
                int action = enemy.getAction(map, player);

                // MOVIMENTO A DESTRA
                if (action == ACTION_GO_RIGHT) {
                    if ((!map->isSolidAt(enemy.getBodyFrontPosition()) && !map->isSolidAt(enemy.getHeadFrontPosition())) &&   // Davanti non è solido
                        (!player.existsAt(enemy.getBodyFrontPosition()) && !player.existsAt(enemy.getHeadFrontPosition())) || // Davanti non c'è il giocatore
                        enemy.getDirection() == DIRECTION_LEFT) {
                        moveRight(map, screen, enemy);
                    }
                }
                // MOVIMENTO A SINISTRA
                else if (action == ACTION_GO_LEFT) {
                    if ((!map->isSolidAt(enemy.getBodyFrontPosition()) && !map->isSolidAt(enemy.getHeadFrontPosition()) &&    // Davanti non è solido
                        (!player.existsAt(enemy.getBodyFrontPosition()) && !player.existsAt(enemy.getHeadFrontPosition())) || // Davanti non c'è il giocatore
                        (enemy.getDirection() == DIRECTION_RIGHT))) {
                        moveLeft(map, screen, enemy);
                    }
                }
                // SALTO
                else if (action == ACTION_JUMP) {
                    if (enemy.isOnTerrain()) {
                        enemy.initJump();
                    }
                }
                // CADUTA
                else if (action == ACTION_FALL) {
                    if (map->isSolidAt(Position(enemy.getBodyPosition().getX(), enemy.getBodyPosition().getY()+1)) &&
                        !map->isSolidAt(Position(enemy.getBodyPosition().getX(), enemy.getBodyPosition().getY()+2)) &&
                        !map->isSolidAt(Position(enemy.getBodyPosition().getX(), enemy.getBodyPosition().getY()+3)) &&
                        !player.existsAt(Position(enemy.getBodyPosition().getX(), enemy.getBodyPosition().getY()+2)) &&
                        !player.existsAt(Position(enemy.getBodyPosition().getX(), enemy.getBodyPosition().getY()+3))) {

                        enemy.setOnTerrain(false);
                        fall(map, screen, enemy);
                    }
                }
                // ATTACCO
                else if (action == ACTION_ATTACK) {
                    if (enemy.isOnTerrain()) {
                        if (enemy.canAttack()) {
                            if (enemy.getWeapon().getType() == WEAPON_MELEE) {
                                melee_attack(map, screen, player, enemy, true);
                            }
                            else {
                                ranged_attack(map, screen, player, enemy, true);
                            }

                            screen.write_at(map, enemy.getWeapon().getTexture(enemy.getDirection()), enemy.getBodyFrontPosition());
                        }
                        else {
                            if (enemy.canReload() && !enemy.getWeapon().hasAmmo()) {
                                enemy.reload();
                            }
                        }
                    }
                }
            } // if (enemy.canRefresh())

            /*** Controlla se il nemico si trova su un terreno solido ***/
            if (!map->isSolidAt(Position(enemy.getBodyPosition().getX(), enemy.getBodyPosition().getY()+1))) {
                enemy.setOnTerrain(false);
            }

            /*** Gestione salto ***/
            if (enemy.canJump()) {
                // Interrompe il salto se raggiunge il soffitto, un doppio blocco solido o il giocatore
                if (enemy.getHeadPosition().getY() <= 0 || 
                    (map->isSolidAt(Position(enemy.getHeadPosition().getX(), enemy.getHeadPosition().getY()-1)) && 
                    map->isSolidAt(Position(enemy.getHeadPosition().getX(), enemy.getHeadPosition().getY()-2))) || 
                    player.existsAt(Position(enemy.getHeadPosition().getX(), enemy.getHeadPosition().getY()-2))) {
                    enemy.stopJump();
                }
                else {
                    jump(map, screen, enemy);
                }
            }
            /*** Gestione caduta ***/
            else if (enemy.canFall()) {
                fall(map, screen, enemy);
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

                        increase_points(screen, player, enemy.getPoints());

                        if (enemy.getMoney() > 0) { // Rilascia i soldi come bonus
                            screen.resetTerrain(
                                map, 
                                map->addBonus(Bonus(Pixel(MONEY_SYMBOL, MONEY_COLOR, BACKGROUND_DEFAULT, false), enemy.getBodyPosition(), 0, enemy.getMoney(), 0))
                            );
                        }
                    }

                    bulletlist.updateCurrent(hit_bullet);
                    map->setBulletList(bulletlist);
                }
            }

            /*** Gestione tempo di ricarica ***/
            enemy.hasReloadFinished();

            /*** Gestione velocità d'attacco ***/
            enemy.hasShootDelayFinished();

            /*** Gestione lava ***/
            if (map->isLava(enemy.getBelowPosition())) {
                if (enemy.isMapEvent()) {
                    if (enemy.canMapEvents()) {
                        enemy.take_damage(1);
                    }
                }
                else {
                    enemy.startMapEvent();
                    enemy.take_damage(1);
                }

                if (enemy.isDead()) {
                    screen.remove_entity(map, enemy);
                }
            }
            else {
                if (enemy.isMapEvent()) {
                    enemy.stopMapEvent();
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
        npclist = map->getNPCList();

        // Visualizza gli NPC sulla mappa
        if (showNPCs) {
            npclist.initIter();
            while(!npclist.isNull()) {
                screen.write_entity(map, npclist.getCurrent());
                npclist.goNext();
            }

            showNPCs = false;
        }

        // Gestisce l'interazione con gli NPC
        if (curr_npc == NULL) {
            if (npclist.inPlayerRange(player.getBodyPosition())) {
                NPC npc = npclist.getCurrent();
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


        /* --------------------------------
           INIZIO GESTIONE GRAVITÀ BONUS
        -------------------------------- */
        bonuslist = map->getBonusList();
        bonuslist.initIter();

        while (!bonuslist.isNull()) {
            Bonus bonus = bonuslist.getCurrent();

            if (bonus.isOnTerrain() && !map->isSolidAt(bonus.getBelowPosition())) {
                bonus.startFall();
            }

            if (!bonus.isOnTerrain() && bonus.canFall()) {
                screen.write_at(map, map->getTerrainAt(bonus.getBodyPosition()), bonus.getBodyPosition());
                bonus.fall();
                screen.write_at(map, bonus.getBody(), bonus.getBodyPosition());
            }

            if (map->isSolidAt(bonus.getBelowPosition())) {
                bonus.stopFall();
            }

            bonus.incCounters();

            bonuslist.updateCurrent(bonus);
            bonuslist.goNext();
        }
        map->setBonusList(bonuslist);
        /* FINE GESTIONE GRAVITÀ BONUS
        ----------------------------- */



        /* -----------------------
           INIZIO GESTIONE BOSS
        ----------------------- */
        if (map->isBossFight()) {
            Boss *boss = map->getBoss();

            if (print_boss_hp) {
                screen.write_boss_hp(*boss);
                print_boss_hp = false;
            }

            if (boss->canRefresh()) {
                Boss old_boss = *boss;

                int action = boss->getAction(map, player);

                if (action == ACTION_GO_RIGHT) {
                    if ((!map->isSolidAt(boss->getBodyFrontPosition()) && !map->isSolidAt(boss->getHeadFrontPosition())) &&
                        (!player.existsAt(boss->getBodyFrontPosition()) && !player.existsAt(boss->getHeadFrontPosition())) ||
                        boss->getDirection() == DIRECTION_LEFT) {
                        screen.remove_boss(map, *boss);
                        boss->goRight();
                        screen.write_boss(map, *boss);
                    }
                }
                else if (action == ACTION_GO_LEFT) {
                    if ((!map->isSolidAt(boss->getBodyFrontPosition()) && !map->isSolidAt(boss->getHeadFrontPosition())) &&
                        (!player.existsAt(boss->getBodyFrontPosition()) && !player.existsAt(boss->getHeadFrontPosition())) ||
                        boss->getDirection() == DIRECTION_RIGHT) {
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
                    if (boss->getType() == BOSS_SUMMONER) {
                        enemylist = map->getEnemyList();

                        for (int i=0; i<boss->getAbilityNum(); i++) {
                            int x = boss->getBodyPosition().getX() + i*3;
                            if (x > GAME_WIDTH) {
                                x = boss->getBodyPosition().getX() - i*3;
                            }
                            int y = boss->getBodyPosition().getY()+3;

                            Enemy new_enemy = createEnemy(map->getDifficulty());
                            new_enemy.setMoney(0);
                            new_enemy.setPosition(Position(x, y));
                            enemylist.add(new_enemy);
                        }

                        map->setEnemyList(enemylist);
                    }
                    else if (boss->getType() == BOSS_MAGE) {
                        if (boss->canAttack()) {
                            Bullet bullet = boss->attack();
                            bullet.setPosition(Position(bullet.getPosition().getX(), player.getBodyPosition().getY()));
                            map->addBullet(bullet);
                            screen.write_bullet(map, player, bullet);
                        }
                    }
                    else if (boss->getType() == BOSS_MELEE) {
                        take_damage(screen, player, boss->getAbilityNum());
                        screen.remove_entity(map, player);

                        if (boss->getDirection() == DIRECTION_LEFT) {
                            player.setPosition(Position(GAME_WIDTH-2, 0));
                        }
                        else {
                            player.setPosition(Position(1, 0));
                        }
                        screen.write_entity(map, player);
                    }
                }
                else {
                    screen.remove_boss(map, *boss);
                    screen.write_boss(map, *boss);
                }

                if (!old_boss.getBodyPosition().equals(boss->getBodyPosition())) {
                    screen.remove_boss(map, old_boss);
                }
            }

            /*** Controlla se il nemico si trova su un terreno solido ***/
            if (!map->isSolidAt(Position(boss->getBodyPosition().getX(), boss->getBodyPosition().getY()+1))) {
                boss->setOnTerrain(false);
            }

            /*** Gestione salto ***/
            if (boss->canJump()) {
                // Interrompe il salto se raggiunge il soffitto, un doppio blocco solido o il giocatore
                if (boss->getHeadPosition().getY() <= 0 ||
                    (map->isSolidAt(Position(boss->getHeadPosition().getX(), boss->getHeadPosition().getY()-1)) &&
                    map->isSolidAt(Position(boss->getHeadPosition().getX(), boss->getHeadPosition().getY()-2))) ||
                    player.existsAt(Position(boss->getHeadPosition().getX(), boss->getHeadPosition().getY()-2)) ) {
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
                    screen.write_boss_hp(*boss);
                    if (boss->isDead()) {
                        screen.remove_boss(map, *boss);

                        increase_points(screen, player, boss->getPoints());

                        screen.resetTerrain(
                            map,
                            map->addBonus(Bonus(Pixel(MONEY_SYMBOL, MONEY_COLOR, BACKGROUND_DEFAULT, false), boss->getBodyPosition(), 0, boss->getMoney(), 0))
                        );

                        map->endBossFight();
                        screen.write_terrain(map);
                        screen.write_entity(map, player);
                        screen.write_bonuses(map);
                        screen.clear_textbox();
                        print_boss_hp = true;
                    }

                    bulletlist.updateCurrent(hit_bullet);
                    map->setBulletList(bulletlist);
                }
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


        /* ------------------------------
            INIZIO GESTIONE PROIETTILI
        ------------------------------ */
        bulletlist = map->getBulletList();
        bulletlist.initIter();

        while (!bulletlist.isNull()) {
            enemylist = map->getEnemyList();
            Bullet bullet = bulletlist.getCurrent();

            if (bullet.canRefresh()) {
                bool boss_exists = false;
                if (map->isBossFight()) {
                    boss_exists = map->getBoss()->existsAt(bullet.getPosition());
                }

                if (!enemylist.existsAt(bullet.getPosition()) && !player.existsAt(bullet.getPosition()) && !boss_exists) {
                    screen.resetTerrain(map, bullet.getPosition());
                }

                if (bullet.travel()) {
                    if (map->getTerrainAt(bullet.getPosition()).isSolid()) {
                        bullet.setRange(-1);
                    }
                    else {
                        screen.write_bullet(map, player, bullet);
                    }
                }
            }

            bullet.incRefresh();

            bulletlist.updateCurrent(bullet);
            map->setBulletList(bulletlist);
        }
        /*  FINE GESTIONE PROIETTILI
        --------------------------- */


        screen.incWeaponboxRotateCounter();
    } // while (!player.isDead())

    screen.game_over(player.getPoints());
    system("pause");
}


void moveLeft(Map *map, Screen screen, Entity &entity) {
    screen.remove_entity(map, entity);
    entity.goLeft();
    screen.write_entity(map, entity);
}

void moveRight(Map *map, Screen screen, Entity &entity) {
    screen.remove_entity(map, entity);
    entity.goRight();
    screen.write_entity(map, entity);
}

void jump(Map *map, Screen screen, Entity &entity) {
    screen.remove_entity(map, entity);
    entity.jump();
    screen.write_entity(map, entity);
}

void fall(Map *map, Screen screen, Entity &entity) {
    screen.remove_entity(map, entity);
    entity.fall();
    screen.write_entity(map, entity);
}

void take_damage(Screen screen, Player &player, int damage) {
    player.take_damage(damage);
    screen.write_hp(player.getHealth());
}

void heal(Screen screen, Player &player, int heal) {
    player.heal(heal);
    screen.write_hp(player.getHealth());
}

void spend_money(Screen screen, Player &player, int money) {
    player.decMoney(money);
    screen.write_money(player.getMoney());
}

void get_money(Screen screen, Player &player, int money) {
    player.incMoney(money);
    screen.write_money(player.getMoney());
}

void increase_points(Screen screen, Player &player, int points) {
    player.incPoints(points);
    screen.write_points(player.getPoints());
}

void ranged_attack(Map *map, Screen screen, Player player, ArmedEntity &entity, bool hostile) {
    Bullet bullet = entity.attack();
    bullet.setHostile(hostile);
    map->addBullet(bullet);
    screen.write_bullet(map, player, bullet);
}

void melee_attack(Map *map, Screen screen, Player player, ArmedEntity &entity, bool hostile) {
    Bullet melee_bullet = entity.attack();
    melee_bullet.setHostile(hostile);
    melee_bullet.setRange(1);
    int range = entity.getWeapon().getRange();

    map->addBullet(melee_bullet);
    int i = 1;
    while (i < range && melee_bullet.getPosition().getX() >= 0 && melee_bullet.getPosition().getX() <= GAME_WIDTH-1) {
        if (entity.getDirection() == DIRECTION_LEFT) {
            melee_bullet.setPosition(Position(melee_bullet.getPosition().getX()-1, melee_bullet.getPosition().getY()));
        }
        else {
            melee_bullet.setPosition(Position(melee_bullet.getPosition().getX()+1, melee_bullet.getPosition().getY()));
        }
        map->addBullet(melee_bullet);
        screen.write_bullet(map, player, melee_bullet);
        i++;
    }

    entity.reload();
}