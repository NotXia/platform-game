#pragma once

#include "Pixel.hpp"
#include "Position.hpp"

class Entity {
	protected:
		int health;
		Pixel head_left;
		Pixel head_right;
		Pixel body;
		Position position;

	public:
		Entity(int health=0, Pixel head_left=Pixel(' ', 0, false), Pixel head_right=Pixel(' ', 0, false), Pixel body=Pixel(' ', 0, false), Position position=Position(0, 0));

		int getHealth();
		Pixel getHeadLeft();
		Pixel getHeadRight();
		Pixel getBody();
		Position getPosition();
		void setPosition(Position position);

		/*
			Prende in input un intero
			Decrementa health di quel valore
		*/
		void take_damage(int damage);

		/*
			Restituisce true se la vita è minore o uguale a zero, false altrimenti
		*/
		bool isDead();

		/*
			Aggiorna position, decrementando di 1 la posizione sull'asse X
		*/
		void goLeft();

		/*
			Aggiorna position, incrementando di 1 la posizione sull'asse X
		*/
		void goRight();
};


