#pragma once
#include <windows.h>
#include "settings.h"
#include "Pixel.hpp"
#include "Entity.hpp"

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
			Prende in input una matrice di Pixel
			Stampa la matrice nell'area di gioco
		*/
		void write_game_area(Pixel terrain[][GAME_HEIGHT]);

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
			Prende in input una posizione e il terreno.
			Imposta posizione position il valore previsto in terrain.
		*/
		void resetTerrain(Pixel terrain[][GAME_HEIGHT], Position position);

		/*
			Prende in input una stringa.
			Inserisce la stringa nell'area di testo (in basso a destra).
		*/
		void write_textbox(const char string[]);

};