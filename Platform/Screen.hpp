#pragma once
#include <windows.h>
#include "settings.h"
#include "Pixel.hpp"
#include "Entity.hpp"
#include "Map.hpp"

// Imposta il template del gioco
class Screen {
	protected:
		HANDLE console;

		int hp_x, hp_y;
		int weapon_x, weapon_y;
		int textBox_x, textBox_y;

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
		void hideCursor();


	public:
		Screen();

		/*
			Inizializza il template del gioco
		*/
		void init();

		/*
			Prende in input una mappa
			Stampa l'area di gioco
		*/
		void write_game_area(Map *map);

		/*
			Prende in input un oggetto di tipo Entity
			Inserisce l'entità nell'area di gioco rivolto verso sinistra
		*/
		void write_entity_left(Entity entity);

		/*
			Prende in input un oggetto di tipo Entity
			Inserisce l'entità nell'area di gioco rivolto verso destra
		*/
		void write_entity_right(Entity entity);

		/*
			Prende in input una posizione e la mappa.
			Imposta in posizione position il valore previsto dalla mappa.
		*/
		void resetTerrain(Map *map, Position position);

		/*
			Prende in input una stringa.
			Inserisce la stringa nell'area di testo (in basso a destra).
		*/
		void write_textbox(const char string[]);

		/*
			Prende in input un Pixel e una Position.
			Imposta il pixel nella posizione indicata.
		*/
		void write_at(Pixel pixel, Position position);

};