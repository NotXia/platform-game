#include <iostream>
#include "Screen.hpp"
#include "settings.h"
using namespace std;

Screen::Screen() {
	this->console = GetStdHandle(STD_OUTPUT_HANDLE);
	hideCursor(); 
	this->hp_x = 1;
	this->hp_y = (GAME_HEIGHT + 2) + GAMEBAR_OFFSET;
	this->weapon_x = 7 + (MAX_LIFE * 2 - 1) + GAMEBAR_PADDING;
	this->weapon_y = this->hp_y;
	this->textBox_x = this->weapon_x + (WEAPON_WIDTH+2) + GAMEBAR_PADDING;
	this->textBox_y = GAME_HEIGHT + 2;
	this->textBox_width = GAME_WIDTH-textBox_x;
	if (textBox_width < TEXTBOX_MIN_WIDTH) {
		textBox_width = TEXTBOX_MIN_WIDTH;
	}
	this->ammobox_width = 2;
	this->weaponbox_width = WEAPON_WIDTH - ammobox_width - 1;

	this->rotation = AnimationTimer(WEAPONBOX_ROTATION_SPEED);
	this->need_rotate = false;
	this->start_index = 0;
}

/*
	Prende in input un intero che rappresenta il colore.
	Imposta il colore dei caratteri successivi in quello indicato come parametro.
*/
void Screen::setColor(int color) {
	SetConsoleTextAttribute(console, color);
}

/*
	Imposta il colore dei caratteri successivi a quello di default della console.
*/
void Screen::resetColor() {
	SetConsoleTextAttribute(console, CONSOLE_DEFAULT);
}

/*
	Prende in input due interi che rappresentano le coordinate.
	Muove il cursore a tali coordinate.
*/
void Screen::moveCursor(int x, int y) {
	COORD coord;
	coord.X = x;
	coord.Y = y;
	SetConsoleCursorPosition(console, coord);
}

/*
	Modifica le variabili di sistema per nascondere il cursore.
*/
void Screen::hideCursor() {
	CONSOLE_CURSOR_INFO cursorInfo;

	GetConsoleCursorInfo(console, &cursorInfo);
	cursorInfo.dwSize = 1;
	cursorInfo.bVisible = false; 
	SetConsoleCursorInfo(console, &cursorInfo);
}

/*
	Inizializza il template del gioco
*/
void Screen::init() {
	system("cls");

	// Area di gioco
	cout <<char(201); // ╔
	for (int i=0; i<GAME_WIDTH; i++) {
		cout <<char(205); // ═
	}
	cout <<char(187) <<endl; // ╗
	for (int i=0; i<GAME_HEIGHT; i++) {
		cout <<char(186); // ║
		moveCursor(GAME_WIDTH+1, 1+i);
		cout <<char(186) <<endl; // ║
	}
	cout <<char(200); // ╚
	for (int i=0; i<GAME_WIDTH; i++) {
		cout <<char(205); // ═
	}
	cout <<char(188); // ╝


	// Barra della vita
	write_hp(MAX_LIFE);

	// Soldi
	write_money(0);

	// Punteggio
	write_points(0);


	// Area textbox arma
	moveCursor(weapon_x, weapon_y);
	cout <<char(218); // ┌
	for (int i=0; i<WEAPON_WIDTH; i++) {
		cout <<char(196); // ─
	}
	cout <<char(191); // ┐
	for (int i=1; i<=WEAPON_HEIGHT+1; i++) {
		moveCursor(weapon_x, weapon_y+i);
		cout <<char(179); // │
		moveCursor(weapon_x+WEAPON_WIDTH+1, weapon_y+i);
		cout <<char(179); // │
	}
	moveCursor(weapon_x, weapon_y+WEAPON_HEIGHT+1);
	cout <<char(192); // ┌
	for (int i=0; i<WEAPON_WIDTH; i++) {
		cout <<char(196); // ─
	}
	cout <<char(217); // ┘


	// Textbox dialogo
	moveCursor(textBox_x, textBox_y);
	cout <<char(218); // ┌
	for (int i=0; i<textBox_width; i++) {
		cout <<char(196); // ─
	}
	cout <<char(191); // ┐
	for (int i=1; i<=TEXTBOX_HEIGHT; i++) {
		moveCursor(textBox_x, textBox_y+i);
		cout <<char(179); // │
		moveCursor(textBox_x+textBox_width+1, textBox_y+i);
		cout <<char(179); // │
	}
	moveCursor(textBox_x, textBox_y+TEXTBOX_HEIGHT+1);
	cout <<char(192); // ┌
	for (int i=0; i<textBox_width; i++) {
		cout <<char(196); // ─
	}
	cout <<char(217); // ┘
	
	resetColor();
}


/**********************************
   INIZIO GESTIONE AREA DI GIOCO
**********************************/

/*
	Prende in input un oggetto Map.
	Stampa l'area di gioco.
*/
void Screen::write_game_area(Map *map) {
	write_terrain(map);
	write_enemies(map);
	write_bonuses(map);
	write_level_number(map);
}

/*
	Prende in input un oggetto Map.
	Stampa il terreno del gioco.
*/
void Screen::write_terrain(Map *map) {
	for (int i=0; i<GAME_HEIGHT; i++) {
		moveCursor(1, 1+i);
		for (int j=0; j<GAME_WIDTH; j++) {
			setColor(map->getTerrainAt(Position(j, i)).getColor());
			cout <<map->getTerrainAt(Position(j, i)).getValue();
		}
	}
	resetColor();
}

/*
	Prende in input un oggetto Map, Pixel e Position.
	Imposta il pixel nella posizione indicata.
	Cambia lo sfondo se necessario.
*/
void Screen::write_at(Map *map, Pixel pixel, Position position) {
	moveCursor(position.getX()+1, position.getY()+1);
	pixel.setBackgroundColor(map->getTerrainAt(position).getBackgroundColor());
	setColor(pixel.getColor());
	cout <<pixel.getValue();
	resetColor();

}

/*
	Prende in input un oggetto Map ed Entity.
	Inserisce l'entità nell'area di gioco.
*/
void Screen::write_entity(Map *map, Entity entity) {
	// Corpo
	write_at(map, entity.getBody(), entity.getBodyPosition());
	
	// Testa
	write_at(map, entity.getHead(), entity.getHeadPosition());
}

/*
	Prende in input un oggetto Map ed Entity.
	Rimuove l'entità dall'area di gioco.
*/
void Screen::remove_entity(Map *map, Entity entity) {
	resetTerrain(map, entity.getBodyPosition());
	resetTerrain(map, entity.getHeadPosition());
}

/*
	Prende in input un oggetto Map, Player e Bullet.
	Inserisce un proiettile nell'area di gioco, se non ci sono altre entità nella sua posizione.
*/
void Screen::write_bullet(Map *map, Player player, Bullet bullet) {
	EnemyList enemylist = map->getEnemyList();
	BonusList bonuslist = map->getBonusList();
	bool boss_exists = false;
	if (map->isBossFight()) {
		boss_exists = map->getBoss()->existsAt(bullet.getPosition());
	}

	if (!enemylist.existsAt(bullet.getPosition()) && !player.existsAt(bullet.getPosition()) && !boss_exists && !bonuslist.pointAt(bullet.getPosition())) {
		write_at(map, bullet.getBody(), bullet.getPosition());
	}
}

/*
	Prende in input un oggetto Map e Position.
	Imposta nella posizione indicata il valore previsto dalla mappa.
*/
void Screen::resetTerrain(Map *map, Position position) {
	BonusList bonuslist = map->getBonusList();
	NPCList npclist = map->getNPCList();

	if (bonuslist.pointAt(position)) {
		write_at(map, bonuslist.getCurrent().getBody(), position);
	}
	else if (npclist.pointAt(position)) {
		write_entity(map, npclist.getCurrent());
	}
	else {
		write_at(map, map->getTerrainAt(position), position);
	}
}

/*
	Prende in input un oggetto Map ed EnemyList.
	Stampa sullo schermo tutti i nemici
*/
void Screen::write_enemies(Map *map) {
	EnemyList list = map->getEnemyList();
	list.initIter();

	while (!list.isNull()) {
		write_entity(map, list.getCurrent());

		list.goNext();
	}
}

/*
	Prende in input un oggetto Map e BonusList.
	Stampa sullo schermo tutti i bonus
*/
void Screen::write_bonuses(Map *map) {
	BonusList list = map->getBonusList();
	list.initIter();

	while (!list.isNull()) {
		Bonus bonus = list.getCurrent();
		write_at(map, bonus.getBody(), bonus.getBodyPosition());

		list.goNext();
	}
}

/*
	Prende in input un oggetto Map e Boss
	Inserisce l'entità nell'area di gioco
*/
void Screen::write_boss(Map *map, Boss boss) {
	// Corpo
	write_at(map, boss.getBody(), boss.getBodyPosition());
	write_at(map, boss.getBody(), boss.getBody2Position());

	// Testa
	write_at(map, boss.getHead(), boss.getHeadPosition());
}

/*
	Prende in input un oggetto Map e Boss
	Rimuove il boss dall'area di gioco
*/
void Screen::remove_boss(Map *map, Boss boss) {
	resetTerrain(map, boss.getBodyPosition());
	resetTerrain(map, boss.getBody2Position());
	resetTerrain(map, boss.getHeadPosition());
}

void Screen::write_weapon(Map *map, ArmedEntity entity) {
	if (!map->isSolidAt(entity.getBodyFrontPosition()) && !entity.getBodyFrontPosition().equals(entity.getBodyPosition())) {
		write_at(map, entity.getWeapon().getTexture(entity.getDirection()), entity.getBodyFrontPosition());
	}
}

/* FINE GESTIONE AREA DI GIOCO
*******************************/


/*****************************
   INIZIO GESTIONE TEXT BOX
*****************************/

/*
	Rimuove tutto il testo nell'area di testo.
*/
void Screen::clear_textbox() {
	int start_x = textBox_x+1;
	int start_y = textBox_y+1;

	resetColor();

	for (int i=0; i<TEXTBOX_HEIGHT; i++) {
		moveCursor(start_x, start_y+i);
		for (int j=0; j<textBox_width; j++) {
			cout <<" ";
		}
	}
}

/*
	Prende in input una stringa.
	Inserisce la stringa nell'area di testo (in basso a destra).
*/
void Screen::write_textbox(const char string[]) {
	clear_textbox();
	int start_x = textBox_x+1;
	int start_y = textBox_y+1;

	moveCursor(start_x, start_y);
	resetColor();
	cout <<string;
}

/*
	Prende in input due oggetti Weapon.
	Inserisce nella textbox il testo previsto quando il giocatore si posiziona sopra un bonus di tipo arma.
*/
void Screen::write_textbox_weaponbonus(Weapon bonus_weapon, Weapon player_weapon) {
	char weapon_name[STRING_LEN];
	bonus_weapon.getName(weapon_name);
	
	clear_textbox();
	int start_x = textBox_x+1;
	int start_y = textBox_y+1;

	resetColor();
	moveCursor(start_x, start_y);
	cout <<"Hai trovato questa arma:";
	moveCursor(start_x, start_y+1);
	cout <<weapon_name;
	moveCursor(start_x, start_y+2);
	cout <<"(" <<bonus_weapon.higherDamage(player_weapon) <<") danni | " 
		 <<"(" <<bonus_weapon.higherAmmo(player_weapon) <<") munizioni | " 
		 <<"(" <<bonus_weapon.higherRange(player_weapon) <<") range";
	moveCursor(start_x, start_y+3);
	cout <<"(" <<bonus_weapon.fasterReload(player_weapon) <<") velocita' ricarica | " 
		 <<"(" <<bonus_weapon.fasterShootRate(player_weapon) <<") cad. di fuoco";
	moveCursor(start_x, start_y+5);
	cout <<"[E] Prendi";
}

/*
	Prende in input un oggetto NPC e un intero.
	Inserisce nella textbox il dialogo di un NPC medico.
*/
void Screen::write_textbox_npc_hp(NPC npc, int missing_hp) {
	char name[STRING_LEN];
	npc.getName(name);

	clear_textbox();
	int start_x = textBox_x+1;
	int start_y = textBox_y+1;

	resetColor();
	moveCursor(start_x, start_y);
	cout <<name <<": Ciao, sono il medico di questo";
	moveCursor(start_x, start_y+1);
	cout <<"villaggio, come posso aiutare?";
	moveCursor(start_x, start_y+3);
	cout <<"[E] Cura 1 (" <<npc.getPriceHP() <<" ";
	setColor(MONEY_COLOR | BG_BLACK);
	cout <<MONEY_SYMBOL;
	resetColor();
	cout <<")";
	moveCursor(start_x, start_y+4);
	cout <<"[Q] Cura tutto (" <<npc.getPriceHP()*missing_hp <<" ";
	setColor(MONEY_COLOR | BG_BLACK);
	cout <<MONEY_SYMBOL;
	resetColor();
	cout <<")";
}

/*
	Prende in input un oggetto NPC e un oggetto Weapon.
	Inserisce nella textbox il dialogo di un NPC mercante.
*/
void Screen::write_textbox_npc_weapon(NPC npc, Weapon player_weapon) {
	char name[STRING_LEN];
	char weapon_name[STRING_LEN];
	npc.getName(name);
	Weapon weapon = npc.getCurrWeapon();
	weapon.getName(weapon_name);

	clear_textbox();
	int start_x = textBox_x+1;
	int start_y = textBox_y+1;
	resetColor();

	if (npc.getWeaponNumber() > 0) {
		moveCursor(start_x, start_y);
		cout <<name <<": Ciao, sono il mercante di questo";
		moveCursor(start_x, start_y+1);
		cout <<"villaggio, ti serve qualcosa?";
		moveCursor(start_x, start_y+2);
		cout <<"< " <<weapon_name <<" (" <<npc.getCurrSelected()+1 <<"/" <<npc.getWeaponNumber() <<") >";
		moveCursor(start_x, start_y+3);
		cout <<"(" <<weapon.higherDamage(player_weapon) <<") danni | "
			<<"(" <<weapon.higherAmmo(player_weapon) <<") munizioni | "
			<<"(" <<weapon.higherRange(player_weapon) <<") range";
		moveCursor(start_x, start_y+4);
		cout <<"(" <<weapon.fasterReload(player_weapon) <<") velocita' ricarica | "
			 <<"(" <<weapon.fasterShootRate(player_weapon) <<") rateo attacco";
		moveCursor(start_x, start_y+6);
		cout <<"[E] Compra (" <<npc.getCurrWeaponPrice() <<" ";
		setColor(MONEY_COLOR | BG_BLACK);
		cout <<MONEY_SYMBOL;
		resetColor();
		cout <<")  [o] Indietro  [p] Avanti";
	}
	else {
		moveCursor(start_x, start_y);
		cout <<name <<": Ciao, sfortunatamente oggi ho";
		moveCursor(start_x, start_y+1);
		cout <<"finito la merce :-(";
	}
}

/*
	Prende in input un oggetto Boss.
	Visualizza nell'area di testo la sua barra della vita.
*/
void Screen::write_boss_hp(Boss boss) {
	int health_len = ((textBox_width * boss.percHealth()) / 100);

	if (health_len > 0) {
		moveCursor(textBox_x+1, textBox_y + 4);

		setColor(FG_DARKRED);
		for (int i=0; i<health_len; i++) {
			cout <<char(178);
		}
		resetColor();
		for (int i=0; i<textBox_width-health_len; i++) {
			cout <<" ";
		}
	}
}

/*
	Prende in input un oggetto Map.
	Stampa il numero del livello.
*/
void Screen::write_level_number(Map *map) {
	moveCursor(textBox_x+2, textBox_y);
	resetColor();

	cout <<char(180) <<" Livello " <<map->getDifficulty() <<"-" <<(map->getLevelNumber() % DIFFICULTY_INCREASE_RATE)+1 <<" " <<char(195);

	resetColor();
}


/* FINE GESTIONE TEXT BOX
**************************/


/********************************
   INIZIO GESTIONE DATI PLAYER
********************************/


/*
	Prende in input un intero.
	Aggiorna la quantità di soldi visualizzata.
*/
void Screen::write_money(int money) {
	moveCursor(hp_x, hp_y + 1);
	resetColor();
	cout <<"Soldi " <<"      ";

	moveCursor(hp_x, hp_y + 1);
	cout <<"Soldi " <<money <<" ";
	setColor(MONEY_COLOR | BG_BLACK);
	cout <<MONEY_SYMBOL;
	resetColor();
}

/*
	Prende in input un intero.
	Aggiorna la quantità di punti visualizzata.
*/
void Screen::write_points(int points) {
	moveCursor(hp_x, hp_y+2);
	resetColor();
	cout <<"Punti " <<points;
}


/*
	Prende in input un intero.
	Aggiorna il numero di cuori visualizzato.
*/
void Screen::write_hp(int hp) {
	if (hp >= 0) {
		resetColor();
		moveCursor(1, hp_y);
		cout <<"HP";
		moveCursor(7, hp_y);
		for (int i=0; i<MAX_LIFE; i++) {
			cout <<"  ";
		}

		moveCursor(7, hp_y);
		setColor(HP_COLOR | BG_BLACK);
		for (int i=0; i<hp; i++) {
			cout <<HP_SYMBOL <<" ";
		}

		setColor(HP_LOSS_COLOR | BG_BLACK);
		for (int i=0; i<MAX_LIFE-hp; i++) {
			cout <<HP_SYMBOL <<" ";
		}
		resetColor();
	}
}

/* FINE GESTIONE DATI PLAYER
*****************************/


/******************************
   INIZIO GESTIONE DATI ARMA
******************************/

/*
	Prende in input un intero.
	Aggiorna la quantità di munizioni visualizzata.
*/
void Screen::write_ammobox(int ammo) {
	moveCursor(weapon_x+WEAPON_WIDTH-ammobox_width, weapon_y+1);
	cout <<"|  ";
	moveCursor(weapon_x+WEAPON_WIDTH-ammobox_width, weapon_y+1);

	if (ammo < 0) {
		cout <<"| R";
	}
	else {
		if (ammo > 9) {
			cout <<"|" <<ammo;
		}
		else {
			cout <<"| " <<ammo;
		}
	}
}

/*
	Prende in input una stringa.
	Scrive il parametro nell'area del nome dell'arma. Se la stringa è troppo lunga, vengono inizializzati i parametri per la rotazione.
*/
void Screen::write_weaponbox(char name[]) {
	moveCursor(weapon_x+1, weapon_y+1);
	resetColor();
	for (int i=0; i<WEAPON_WIDTH; i++) {
		cout <<" ";
	}
	moveCursor(weapon_x+1, weapon_y+1);

	strncpy(weaponbox_text, name, STRING_LEN);
	start_index = 0;
	if (strlen(name) > weaponbox_width) {
		need_rotate = true;
		strcat(weaponbox_text, "   ");
		for (int i=0; i<weaponbox_width; i++) {
			cout <<name[i];
		}
	}
	else {
		need_rotate = false;
		cout <<name;
	}
}

/*
	Prende in input un oggetto Weapon.
	Scrive il nome dell'arma e il numero di munizioni.
*/
void Screen::write_weaponInfo(Weapon weapon) {
	char weapon_name[STRING_LEN];
	weapon.getName(weapon_name);
	write_weaponbox(weapon_name);
	write_ammobox(weapon.getCurrAmmo());
}

/*
	Incrementa di 1 rotation_counter.
*/
void Screen::incWeaponboxRotateCounter() {
	if (need_rotate) {
		rotation.incTimer();
	}
}

/*
	Indica se è possibile avanzare con l'animazione della rotazione del nome dell'arma.
*/
bool Screen::canRotateWeaponbox() {
	return rotation.limit() && need_rotate;
}

/*
	Incrementa start_index e stampa nell'area dell'arma la stringa weaponbox_text a partire da quell'indice.
	Se start_index supera la dimensione della stringa, ricomincia da capo.
*/
void Screen::rotate_weaponbox() {
	start_index++;
	if (start_index > strlen(weaponbox_text)) {
		start_index = 0;
	}

	moveCursor(weapon_x+1, weapon_y+1);
	for (int i=start_index; i<start_index+weaponbox_width; i++) {
		if (i < strlen(weaponbox_text)) {
			cout <<weaponbox_text[i];
		}
		else {
			cout <<weaponbox_text[i-strlen(weaponbox_text)];
		}
	}
}

/* FINE GESTIONE DATI ARMA
***************************/


/*
	Prende in input i punti.
	Gestisce la schermata di fine partita.
*/
void Screen::game_over(int points) {
	system("cls");
	resetColor();
	moveCursor(0, 0);
	cout <<" _____           __  __ ______    ______      ________ _____" <<endl
		 <<"/ ____|    /\\   |  \\/  |  ____|  / __ \\ \\    / /  ____|  __ \\" <<endl
		 <<"| |  __   /  \\  | \\  / | |__    | |  | \\ \\  / /| |__  | |__) |" <<endl
		 <<"| | |_ | / /\\ \\ | |\\/| |  __|   | |  | |\\ \\/ / |  __| |  _  /" <<endl
		 <<"| |__| |/ ____ \\| |  | | |____  | |__| | \\  /  | |____| | \\ \\" <<endl
		 <<"\\_____ /_/    \\_\\_|  |_|______|  \\____/   \\/   |______|_|  \\_\\" <<endl;

	cout <<endl <<endl;

	cout <<"Hai totalizzato " <<points <<" punti" <<endl;
}
