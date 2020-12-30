#pragma once
#include <windows.h>
#include "settings.h"
#include "Pixel.hpp"
#include "Entity.hpp"
#include "Map.hpp"
#include "EnemyList.hpp"

class Screen {
	protected:
		const int GAMEBAR_OFFSET = 3;
		const int GAMEBAR_PADDING = 4;

		const int WEAPON_WIDTH = 20;
		const int WEAPON_HEIGHT = 1;

		const int TEXTBOX_HEIGHT = 5;
		const int TEXTBOX_MIN_WIDTH = 15;

		const int WEAPONBOX_ROTATION_SPEED = 8000;


		HANDLE console;

		int hp_x, hp_y;
		int weapon_x, weapon_y;
		int textBox_x, textBox_y, textBox_width;
		int weaponbox_width, ammobox_width;

		int start_index;
		char weaponbox_text[STRING_LEN];
		bool need_rotate;
		int rotation_counter;

		/*
			Prende in input un intero che rappresenta il colore 
			Imposta il colore dei caratteri successivi in quello indicato come parametro
		*/
		void setColor(int color);

		/*
			Imposta il colore dei caratteri successivi a quello di default della console
		*/
		void resetColor();

		/*
			Prende in input due interi che rappresentano le coordinate
			Muove il cursore a tali coordinate
		*/
		void moveCursor(int x, int y);

		/*
			Modifica le variabili di sistema per nascondere il cursore
		*/
		void hideCursor();


	public:
		Screen();

		/*
			Inizializza il template del gioco
		*/
		void init();


		/**********************************
		   INIZIO GESTIONE AREA DI GIOCO
		**********************************/

		/*
			Prende in input un oggetto Map
			Stampa l'area di gioco
		*/
		void write_game_area(Map *map);

		/*
			Prende in input un oggetto Map
			Stampa il terreno del gioco
		*/
		void write_terrain(Map *map);

		/*
			Prende in input un Pixel e una Position.
			Imposta il pixel nella posizione indicata.
		*/
		void write_at(Pixel pixel, Position position);

		/*
			Prende in input un oggetto di tipo Entity
			Inserisce l'entità nell'area di gioco
		*/
		void write_entity(Entity entity);

		/*
			Prende in input una posizione e la mappa.
			Imposta in posizione position il valore previsto dalla mappa.
		*/
		void resetTerrain(Map *map, Position position);

		/*
			Prende in input un oggetto EnemyList.
			Stampa sullo schermo tutti i nemici
		*/
		void write_enemies(EnemyList list);

		/*
			Prende in input un oggetto BonusList.
			Stampa sullo schermo tutti i bonus
		*/
		void write_bonuses(BonusList list);

		/* FINE GESTIONE AREA DI GIOCO  
		*******************************/


		/*****************************
		   INIZIO GESTIONE TEXT BOX   
		*****************************/
		/*
			Rimuove tutto il testo nell'area di testo
		*/
		void clear_textbox();

		/*
			Prende in input una stringa.
			Inserisce la stringa nell'area di testo.
		*/
		void write_textbox(const char string[]);

		/*
			Prende in input un oggetto Weapon.
			Inserisce nella textbox il testo previsto quando il giocatore si posiziona sopra un bonus di tipo arma.
		*/
		void write_textbox_weaponbonus(Weapon bonus_weapon, Weapon player_weapon);

		/* FINE GESTIONE TEXT BOX  
		**************************/


		/********************************
		   INIZIO GESTIONE DATI PLAYER
		********************************/

		/*
			Prende in input un intero
			Aggiorna la quantità di soldi visualizzata
		*/
		void write_money(int money);

		/*
			Prende in input un intero
			Aggiorna la quantità di punti visualizzata
		*/
		void write_points(int points);

		/*
			Prende in input un intero.
			Aggiorna il numero di cuori visualizzato
		*/
		void write_hp(int hp);

		/* FINE GESTIONE DATI PLAYER  
		*****************************/


		/******************************
		   INIZIO GESTIONE DATI ARMA   
		******************************/

		/*
			Prende in input un intero.
			Aggiorna la quantità di munizioni visualizzata
		*/
		void write_ammobox(int ammo);

		/*
			Prende in input una stringa
			Scrive il parametro nell'area del nome dell'arma. Se la stringa è troppo lunga, vengono inizializzati i parametri per la rotazione
		*/
		void write_weaponbox(char name[]);

		/*
			Incrementa di 1 rotation_counter
		*/
		void incWeaponboxRotateCounter();

		/*
			Restituisce true se rotation_counter ha raggiunto WEAPONBOX_ROTATION_SPEED e need_rotate è true
		*/
		bool canRotateWeaponbox();

		/*
			Incrementa start_index e stampa nell'area dell'arma la stringa weaponbox_text a partire da quell'indice.
			Se start_index supera la dimensione della stringa, ricomincia da capo.
		*/
		void rotate_weaponbox();

		/* FINE GESTIONE DATI ARMA  
		***************************/
};