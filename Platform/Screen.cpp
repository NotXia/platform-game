#include "Screen.hpp"
#include <iostream>
#include "colors.h"
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

	this->rotation_counter = 0;
	this->need_rotate = false;
	this->start_index = 0;
}

/*
	Prende in input un intero che rappresenta il colore
	Imposta il colore dei caratteri successivi in quello indicato come parametro
*/
void Screen::setColor(int color) {
	SetConsoleTextAttribute(console, color);
}

/*
	Imposta il colore dei caratteri successivi a quello di default della console
*/
void Screen::resetColor() {
	SetConsoleTextAttribute(console, CONSOLE_DEFAULT);
}

/*
	Prende in input due interi che rappresentano le coordinate
	Muove il cursore a tali coordinate
*/
void Screen::moveCursor(int x, int y) {
	COORD coord;
	coord.X = x;
	coord.Y = y;
	SetConsoleCursorPosition(console, coord);
}

/*
	Modifica le variabili di sistema per nascondere il cursore
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


	// Area selettore arma
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


	// Text box
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
	Prende in input un oggetto Map
	Stampa l'area di gioco
*/
void Screen::write_game_area(Map *map) {
	write_terrain(map);
	write_enemies(map, map->getEnemyList());
	write_bonuses(map, map->getBonusList());
}

/*
	Prende in input un oggetto Map
	Stampa il terreno del gioco
*/
void Screen::write_terrain(Map *map) {
	for (int i=1; i<=GAME_HEIGHT; i++) {
		moveCursor(1, i);
		for (int j=1; j<=GAME_WIDTH; j++) {
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
	moveCursor(position.getX(), position.getY());
	pixel.setBackgroundColor(map->getTerrainAt(position).getBackgroundColor());
	setColor(pixel.getColor());
	cout <<pixel.getValue();
	resetColor();

}

/*
	Prende in input un oggetto Map ed Entity
	Inserisce l'entità nell'area di gioco
*/
void Screen::write_entity(Map *map, Entity entity) {
	// Corpo
	write_at(map, entity.getBody(), entity.getBodyPosition());
	
	// Testa
	write_at(map, entity.getHead(), entity.getHeadPosition());
}

/*
	Prende in input un oggetto Map e Bullet
	Inserisce un proiettile nell'area di gioco
*/
void Screen::write_bullet(Map *map, Bullet bullet) {
	write_at(map, bullet.getTexture(), bullet.getPosition());
}

/*
	Prende in input una posizione e la mappa.
	Imposta in posizione position il valore previsto dalla mappa.
*/
void Screen::resetTerrain(Map *map, Position position) {
	BonusList bonuslist = map->getBonusList();

	if (bonuslist.pointAt(position)) {
		write_at(map, bonuslist.getCurrent().getBody(), position);
	}
	else {
		write_at(map, map->getTerrainAt(position), position);
	}
}

/*
	Prende in input un oggetto Map ed EnemyList.
	Stampa sullo schermo tutti i nemici
*/
void Screen::write_enemies(Map *map, EnemyList list) {
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
void Screen::write_bonuses(Map *map, BonusList list) {
	list.initIter();

	while (!list.isNull()) {
		Bonus bonus = list.getCurrent();
		write_at(map, bonus.getBody(), bonus.getBodyPosition());

		list.goNext();
	}
}

/* FINE GESTIONE AREA DI GIOCO
*******************************/


/*****************************
   INIZIO GESTIONE TEXT BOX
*****************************/

/*
	Rimuove tutto il testo nell'area di testo
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
	Prende in input un oggetto Weapon.
	Inserisce nella textbox il testo previsto quando il giocatore si posiziona sopra un bonus di tipo arma.
*/
void Screen::write_textbox_weaponbonus(Weapon bonus_weapon, Weapon player_weapon) {
	clear_textbox();
	int start_x = textBox_x+1;
	int start_y = textBox_y+1;

	resetColor();
	moveCursor(start_x, start_y);
	cout <<"Hai trovato questa arma:";
	moveCursor(start_x, start_y+1);
	cout <<bonus_weapon.getName();
	moveCursor(start_x, start_y+2);
	cout <<"(" <<bonus_weapon.higherDamage(player_weapon) <<") danni | " 
		 <<"(" <<bonus_weapon.higherAmmo(player_weapon) <<") munizioni | " 
		 <<"(" <<bonus_weapon.higherRange(player_weapon) <<") range";
	moveCursor(start_x, start_y+3);
	cout <<"(" <<bonus_weapon.fasterReload(player_weapon) <<") tempo ricarica | " 
		 <<"(" <<bonus_weapon.fasterShootRate(player_weapon) <<") velocita' attacco";
	moveCursor(start_x, start_y+5);
	cout <<"[E] Prendi";
}

/* FINE GESTIONE TEXT BOX
**************************/


/********************************
   INIZIO GESTIONE DATI PLAYER
********************************/

/*
	Prende in input un intero
	Aggiorna la quantità di soldi visualizzata
*/
void Screen::write_money(int money) {
	moveCursor(hp_x, hp_y + 1);
	cout <<"Soldi " <<money <<" ";
	setColor(FG_DARKYELLOW | BG_BLACK);
	cout <<MONEY_SYMBOL;
	resetColor();
}

/*
	Prende in input un intero
	Aggiorna la quantità di punti visualizzata
*/
void Screen::write_points(int points) {
	moveCursor(hp_x, hp_y+2);
	resetColor();
	cout <<"Punti " <<points;
}


/*
	Prende in input un intero.
	Aggiorna il numero di cuori visualizzato
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
		setColor(HP_COLOR);
		for (int i=0; i<hp; i++) {
			cout <<HP_SYMBOL <<" ";
		}

		setColor(FG_GREY | BG_BLACK);
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
	Prende in input una stringa
	Scrive il parametro nell'area del nome dell'arma. Se la stringa è troppo lunga, vengono inizializzati i parametri per la rotazione
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
	Incrementa di 1 rotation_counter
*/
void Screen::incWeaponboxRotateCounter() {
	if (need_rotate) {
		rotation_counter++;
		if (rotation_counter > WEAPONBOX_ROTATION_SPEED) {
			rotation_counter = 0;
		}
	}
}

/*
	Restituisce true se rotation_counter ha raggiunto WEAPONBOX_ROTATION_SPEED e need_rotate è true
*/
bool Screen::canRotateWeaponbox() {
	return rotation_counter >= WEAPONBOX_ROTATION_SPEED && need_rotate;
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

/*
	Prende in input un intero.
	Aggiorna la quantità di munizioni visualizzata
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

/* FINE GESTIONE DATI ARMA
***************************/