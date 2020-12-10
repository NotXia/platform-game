#pragma once

#include "Pixel.hpp"
#include "Position.hpp"

const bool DIRECTION_LEFT = false;
const bool DIRECTION_RIGHT = true;

class Entity {
	protected:
		int health;
		Pixel head_left;
		Pixel head_right;
		Pixel body;
		Position position;
		bool direction; // false = left | true = right

		int jump_status; 
		bool is_jumping;
		bool can_jump;

	public:
		Entity(int health=0, Pixel head_left=Pixel(' ', 0, false), Pixel head_right=Pixel(' ', 0, false), Pixel body=Pixel(' ', 0, false), Position position=Position(0, 0));

		int getHealth();
		Pixel getHeadLeft();
		Pixel getHeadRight();
		Pixel getBody(); 
		bool getDirection();
		Position getBodyPosition();
		Position getHeadPosition();
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

		/*
			Inizializza i parametri per il salto dell'entità
		*/
		void initJump();

		bool getCanJump();
		void setCanJump(bool can_jump);
		bool isJumping();

		/*
			Se jump_status è inferiore all'altezza massima di salto, aggiorna position decrementando di 1 la posizione sull'asse Y.
			Altrimenti interrompe il salto.
		*/
		bool jump();

		/*
			Imposta i parametri di salto dell'entità in modo da interromperlo
		*/
		void stopJump();

		/*
			Aggiorna position incrementando di 1 la posizione sull'asse Y.
		*/
		void fall();
};


