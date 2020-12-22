#pragma once
#include "Pixel.hpp"
#include "Position.hpp"

const bool BULLET_LEFT = false;
const bool BULLET_RIGHT = true;

class Bullet {
	protected:
		Pixel textureLeft;
		Pixel textureRight;
		int damage;
		int range;
		Position position;
		bool hostile;
		bool direction;

		int refreshTime, currRefresh;

	public:
		Bullet(Pixel texture=Pixel(), int damage=1, int range=1, Position position=Position(), bool direction=BULLET_LEFT);
		Bullet(Pixel textureLeft, Pixel textureRight, int damage, int range, Position position, bool direction);

		Pixel getTexture();
		int getDamage();
		int getRange();
		void setHostile(bool hostile);
		bool isHostile();
		Position getPosition();
		void setPosition(Position position);
		void setDirection(bool direction);

		/*
			Restituisce true se il range è maggiore di 0, false altrimenti
		*/
		bool canTravel();

		/*
			Se range è maggiore di 0: decrementa range e muove la posizione di 1 (in base all direzione).
			In caso la posizione dovesse superare i limiti, range viene impostato ad un valore nullo.
			Restituisce true se il range è maggiore di 0, false altrimenti.
		*/
		bool travel();

		/*
			Indica se è possibile aggiornare lo stato del Bullet
		*/
		bool canRefresh();

		/*
			Incrementa  currRefresh di 1. Se supera refreshTime, viene azzerato
		*/
		void incRefresh();

		/*
			Imposta range ad un valore negativo e restituisce il danno del proiettile
		*/
		int hit();

};