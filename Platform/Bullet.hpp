#pragma once
#include "Block.hpp"
#include "AnimationTimer.hpp"
#include "settings.h"

const bool BULLET_LEFT = false;
const bool BULLET_RIGHT = true;

class Bullet : public Block {
	protected:
		int damage;
		int range;
		bool hostile;
		bool direction;

		AnimationTimer refresh;

	public:
		Bullet(Pixel texture=Pixel(), int damage=1, int range=1, Position position=Position(), bool direction=BULLET_LEFT, int speed=RANGED_BULLET_SPEED);

		int getDamage();
		int getRange();
		void setRange(int range);
		void setHostile(bool hostile);
		bool isHostile();
		Position getPosition();
		void setPosition(Position position);
		void setDirection(bool direction);

		/*
			Restituisce true se il range è maggiore di 0, false altrimenti.
			Indica se il proiettile può continuare ad avanzare.
		*/
		bool canTravel();

		/*
			Se range è maggiore di 0: decrementa range e muove la posizione di 1 (in base alla direzione).
			In caso la posizione dovesse superare i limiti della mappa, range viene impostato ad un valore nullo.
			Restituisce true se il range è maggiore di 0, false altrimenti.
		*/
		bool travel();

		/*
			Indica se è possibile aggiornare lo stato del Bullet.
		*/
		bool canRefresh();

		/*
			Incrementa il timer refresh di 1.
		*/
		void incRefresh();

		/*
			Imposta range ad un valore negativo e restituisce il danno del proiettile.
		*/
		int hit();

		/*
			Prende in input un oggetto Bullet.
			Confronta l'oggetto corrente con il parametro e dice se sono uguali.
		*/
		bool equals(Bullet bullet);

};