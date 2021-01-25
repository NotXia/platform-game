#pragma once
#include "Enemy.hpp"
#include "Player.hpp"

const int BOSS_SUMMONER = 0;
const int BOSS_MAGE = 1;
const int BOSS_MELEE = 2;

class Boss : public Enemy {
	protected:
		int type;

		int phase, max_phase;

		int ability_num, ability_max;
		AnimationTimer down_time;

		AnimationTimer pause_time;


	public:
		Boss(int health=0, int points=0, int money=0, Pixel head_left=Pixel(), Pixel head_right=Pixel(), Pixel body=Pixel(), Position position=Position(), Weapon weapon=Weapon(), int type=0, int ability_num=0, int ability_max=0);

		int getType();
		int getPhase();
		int getAbilityNum();
		void setAbilityNum(int num_enemies); // Imposta num_enemies, rispettando il limite imposta da max_enemies
		Position getBody2Position();
		Position getHeadPosition();

		/*
			Prende in input un oggetto Position.
			Restituisce true se il boss � presente in quella posizione.
		*/
		bool existsAt(Position position);

		/*
			Incrementa phase di 1.
			Se supera max_phase, azzera phase.
		*/
		void nextPhase();

		/*
			Prende in input un oggetto Map e un oggetto Player.
			Restituisce il codice dell'azione da eseguire
		*/
		int getAction(Map *map, Player player);

		void incCounters();
};

