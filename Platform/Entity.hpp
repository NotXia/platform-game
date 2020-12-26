#pragma once

#include "Pixel.hpp"
#include "Position.hpp"
#include "Block.hpp"

const bool DIRECTION_LEFT = false;
const bool DIRECTION_RIGHT = true;

class Entity : public Block {
	protected:
		int health;
		Pixel head_left;
		Pixel head_right;
		bool direction; // false = left | true = right
		bool can_move;

		bool is_jumping;
		int jump_status; 
		int jump_loop_counter;

		bool on_terrain;
		int fall_loop_counter;


		/*
			Se is_jumping è true: incrementa jump_loop_counter di 1.
			Se supera il limite impostato, viene resettato.
		*/
		void incJumpLoopCounter();

		/*
			Se on_terrain è false: incrementa fall_loop_counter di 1.
			Se supera il limite impostato, viene resettato.
		*/
		void incFallLoopCounter();

	public:
		Entity(int health=0, Pixel head_left=Pixel(' ', 0, false), Pixel head_right=Pixel(' ', 0, false), Pixel body=Pixel(' ', 0, false), Position position=Position(0, 0));

		int getHealth();
		Pixel getHead();
		bool getDirection();
		Position getHeadPosition();
		Position getFrontPosition(); // Restituisce le coordinate davanti al corpo (stabilito in base alla direzione)
		Position getBackPosition(); // Restituisce le coordinate dietro al corpo (stabilito in base alla direzione)
		void setPosition(Position position);
		void setCanMove(bool can_move);
		bool getCanMove();
		bool isOnTerrain();
		void setOnTerrain(bool on_terrain);


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
			Se il valore di attuale direction è DIRECTION_LEFT, aggiorna position decrementando di 1 la posizione sull'asse X
			Aggiorna direction a DIRECTION_LEFT.
		*/
		void goLeft();

		/*
			Se il valore di attuale direction è DIRECTION_RIGHT, aggiorna position incrementando di 1 la posizione sull'asse X
			Aggiorna direction a DIRECTION_RIGHT.
		*/
		void goRight();


		/**************************
		   INIZIO GESTIONE SALTO   
		**************************/

		bool isJumping();

		/*
			Inizializza i parametri per il salto dell'entità
		*/
		void initJump();

		/*
			Se jump_status è inferiore all'altezza massima di salto, aggiorna position decrementando di 1 la posizione sull'asse Y.
			Altrimenti interrompe il salto.
		*/
		void jump();

		/*
			Imposta i parametri di salto dell'entità in modo da interromperlo
		*/
		void stopJump();

		/*
			Restituisce true quando è possibile avanzare nell'animazione del salto
		*/
		bool canJump();

		/* FINE GESTIONE SALTO
		************************/


		/***************************
		   INIZIO GESTIONE CADUTA   
		***************************/

		/*
			Aggiorna position incrementando di 1 la posizione sull'asse Y.
		*/
		void fall();

		/*
			Restituisce true quando è possibile avanzare nell'animazione della caduta
		*/
		bool canFall();

		/* FINE GESTIONE CADUTA
		************************/


		/*
			Incrementa i vari contatori
		*/
		void incCounters();
};


