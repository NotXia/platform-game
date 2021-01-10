#pragma once
#include "Enemy.hpp"
#include "Player.hpp"

const int BOSS_TYPE1 = 0;

class Boss : public Enemy {
	protected:
		int type;

		int phase, max_phase;

		int ability_num, ability_max;
		AnimationTimer down_time;


		void terrain_type1(Pixel terrain[GAME_WIDTH][GAME_HEIGHT]);
	public:
		Boss(int health=0, int points=0, int money=0, Pixel head_left=Pixel(), Pixel head_right=Pixel(), Pixel body=Pixel(), Position position=Position(), Weapon weapon=Weapon(), int type=0, int max_phase=0, int ability_num=0, int ability_max=0, int down_time=0, int jump_force=0);

		bool getType();
		int getPhase();
		int getAbilityNum();
		void setAbilityNum(int num_enemies); // Imposta num_enemies, rispettando il limite imposta da max_enemies
		Position getBody2Position();
		Position getHeadPosition();

		/*
			Incrementa phase di 1.
			Se supera max_phase, azzera phase.
		*/
		void nextPhase();

		/*
			Prende in input un oggetto Position.
			Restituisce true se il boss è presente in quella posizione
		*/
		bool existsAt(Position position);

		/*
			Prende in input una matrice di Pixel.
			Inizializza la matrice con la mappa del boss.
		*/
		void initTerrain(Pixel terrain[GAME_WIDTH][GAME_HEIGHT]);

		/*
			Prende in input un oggetto Map e un oggetto Player.
			Restituisce il codice dell'azione da eseguire
		*/
		int getAction(Map *map, Player player);

		/*
			Incrementa i vari contatori per le animazioni
		*/
		void incCounters();
};

