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
		bool can_move;

		int jump_status; 
		bool is_jumping;
		bool on_terrain;
		int jump_loop_counter;
		int fall_loop_counter;


	public:
		Entity(int health=0, Pixel head_left=Pixel(' ', 0, false), Pixel head_right=Pixel(' ', 0, false), Pixel body=Pixel(' ', 0, false), Position position=Position(0, 0));

		int getHealth();
		Pixel getHead();
		Pixel getBody(); 
		bool getDirection();
		Position getBodyPosition();
		Position getHeadPosition();
		Position getFrontPosition(); // Restituisce le coordinate davanti al corpo (stabilito in base alla direzione)
		Position getBackPosition(); // Restituisce le coordinate dietro al corpo (stabilito in base alla direzione)
		void setPosition(Position position);
		void setCanMove(bool can_move);
		bool getCanMove();
		bool isOnTerrain();
		void setOnTerrain(bool on_terrain);
		bool isJumping();


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

		/*
			Incrementa jump_loop_counter di 1.
			Se supera il limite impostato, viene resettato.
		*/
		void incJumpLoopCounter();

		/*
			Resetta jump_loop_counter
		*/
		void resetJumpLoopCounter();

		/*
			Incrementa fall_loop_counter di 1.
			Se supera il limite impostato, viene resettato.
		*/
		void incFallLoopCounter();

		/*
			Resetta fall_loop_counter
		*/
		void resetFallLoopCounter();

		/*
			Restituisce true quando è possibile avanzare nell'animazione del salto
		*/
		bool canJump();

		/*
			Restituisce true quando è possibile avanzare nell'animazione della caduta
		*/
		bool canFall();
};


