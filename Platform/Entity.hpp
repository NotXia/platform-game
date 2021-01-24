#pragma once

#include "Pixel.hpp"
#include "Position.hpp"
#include "Block.hpp"
#include "AnimationTimer.hpp"

const bool DIRECTION_LEFT = false;
const bool DIRECTION_RIGHT = true;

class Entity : public Block {
	protected:
		int health, max_health;
		Pixel head_left;
		Pixel head_right;
		bool direction; // false = left | true = right
		bool can_move;

		bool is_jumping;
		int jump_status, max_jump_height; 
		AnimationTimer jump_animation;

		bool on_terrain;
		AnimationTimer fall_animation;


	public:
		Entity(int health=0, Pixel head_left=Pixel(), Pixel head_right=Pixel(), Pixel body=Pixel(), Position position=Position());

		int getHealth();
		Pixel getHead();
		bool getDirection();
		Position getHeadPosition();
		Position getBodyFrontPosition(); // Restituisce le coordinate davanti alla testa (stabilito in base alla direzione)
		Position getHeadFrontPosition(); // Restituisce le coordinate davanti al corpo (stabilito in base alla direzione)
		Position getBackPosition();		 // Restituisce le coordinate dietro al corpo (stabilito in base alla direzione)
		void setPosition(Position position);
		void setCanMove(bool can_move);
		bool getCanMove();
		bool isOnTerrain();
		void setOnTerrain(bool on_terrain);


		/*
			Prende in input un intero.
			Decrementa health di quel valore.
		*/
		void take_damage(int damage);

		/*
			Restituisce true se la vita è minore o uguale a zero, false altrimenti.
		*/
		bool isDead();

		/*
			Se il valore di attuale direction è DIRECTION_LEFT, aggiorna position decrementando di 1 la posizione sull'asse X.
			Aggiorna direction a DIRECTION_LEFT.
		*/
		void goLeft();

		/*
			Se il valore di attuale direction è DIRECTION_RIGHT, aggiorna position incrementando di 1 la posizione sull'asse X.
			Aggiorna direction a DIRECTION_RIGHT.
		*/
		void goRight();

		/*
			Restituisce la quantità di vita mancante rispetto al massimo.
		*/
		int getMissingHp();

		/*
			Restituisce la quantità di vita sottoforma di percentuale.
		*/
		int percHealth();

		/*
			Prende in input un oggetto Position.
			Restituisce true se la posizione presa in input coincide con una parte dell'entità.
		*/
		bool existsAt(Position position);


		/**************************
		   INIZIO GESTIONE SALTO   
		**************************/

		bool isJumping();

		/*
			Inizializza i parametri per il salto dell'entità.
		*/
		void initJump();

		/*
			Se jump_status è inferiore all'altezza massima di salto, aggiorna position decrementando di 1 la posizione sull'asse Y.
			Altrimenti interrompe il salto.
		*/
		void jump();

		/*
			Imposta i parametri di salto dell'entità in modo da interromperlo.
		*/
		void stopJump();

		/*
			Restituisce true quando è possibile avanzare nell'animazione del salto.
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
			Restituisce true quando è possibile avanzare nell'animazione della caduta.
		*/
		bool canFall();

		/* FINE GESTIONE CADUTA
		************************/


		/*
			Incrementa i vari contatori.
		*/
		void incCounters();

};


