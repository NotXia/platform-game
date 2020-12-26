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
	int textBox_height = 2*GAMEBAR_OFFSET + 3;
	int textBox_width = GAME_WIDTH-textBox_x;
	if (textBox_width < TEXTBOX_MIN_WIDTH) {
		textBox_width = TEXTBOX_MIN_WIDTH;
	}

	moveCursor(textBox_x, textBox_y);
	cout <<char(218); // ┌
	for (int i=0; i<textBox_width; i++) {
		cout <<char(196); // ─
	}
	cout <<char(191); // ┐
	for (int i=0; i<textBox_height-2; i++) {
		textBox_y++;
		moveCursor(textBox_x, textBox_y);
		cout <<char(179); // │
		moveCursor(textBox_x+textBox_width+1, textBox_y);
		cout <<char(179); // │
	}
	moveCursor(textBox_x, textBox_y+1);
	cout <<char(192); // ┌
	for (int i=0; i<textBox_width; i++) {
		cout <<char(196); // ─
	}
	cout <<char(217); // ┘
	
	textBox_y = textBox_y - textBox_height + 2;
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
	Prende in input un Pixel e una Position.
	Imposta il pixel nella posizione indicata.
*/
void Screen::write_at(Pixel pixel, Position position) {
	moveCursor(position.getX(), position.getY());
	setColor(pixel.getColor());
	cout <<pixel.getValue();
	resetColor();
}

/*
	Prende in input un oggetto di tipo Entity
	Inserisce l'entità nell'area di gioco
*/
void Screen::write_entity(Entity entity) {
	// Corpo
	write_at(entity.getBody(), entity.getBodyPosition());
	
	// Testa
	write_at(entity.getHead(), entity.getHeadPosition());
}

/*
	Prende in input una posizione e la mappa.
	Imposta in posizione position il valore previsto dalla mappa.
*/
void Screen::resetTerrain(Map *map, Position position) {
	write_at(map->getTerrainAt(position), position);
}

/*
	Prende in input un oggetto EnemyList.
	Stampa sullo schermo tutti i nemici
*/
void Screen::write_enemies(EnemyList list) {
	list.initIter();

	while (!list.isNull()) {
		write_entity(list.getCurrent());

		list.goNext();
	}
}

/* FINE GESTIONE AREA DI GIOCO
*******************************/


/*****************************
   INIZIO GESTIONE TEXT BOX
*****************************/

/*
	Prende in input una stringa.
	Inserisce la stringa nell'area di testo (in basso a destra).
*/
void Screen::write_textbox(const char string[]) {
	int start_x = textBox_x+1;
	int start_y = textBox_y+1;

	moveCursor(start_x, start_y);
	resetColor();
	cout <<string;
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
	cout <<char(207);
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
			cout <<char(3) <<" "; // ♥
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