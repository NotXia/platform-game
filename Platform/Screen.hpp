#pragma once
#include <windows.h>
#include "settings.h"
#include "Pixel.hpp"
#include "Entity.hpp"

// Imposta il template del gioco
class Screen {
	protected:
		HANDLE console;

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
};