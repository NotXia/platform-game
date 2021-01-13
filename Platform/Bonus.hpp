#pragma once
#include "Block.hpp"
#include "Weapon.hpp"
#include <iostream>

const int BONUS_TYPE_MONEY = 0;
const int BONUS_TYPE_HP = 1;
const int BONUS_TYPE_WEAPON = 2;

class Bonus : public Block {
	protected:
		int points;
		int money;
		int hp;
		Weapon *weapon;

		bool on_terrain;
		AnimationTimer fall_animation;

	public:
		Bonus(Pixel body=Pixel(), Position position=Position(), int points=0, int money=1, int hp=0, Weapon *weapon=NULL);

		int getPoints();
		void setPosition(Position position);
		Weapon getWeapon();
		void setWeapon(Weapon weapon);

		/*
			Restituisce il tipo del bonus in base ai campi impostati
		*/
		int getType();

		/*
			Restituisce il contenuto del bonus in base al tipo (vita o soldi).
		*/
		int getBonus();

		/****************************
			INIZIO GESTIONE CADUTA
		****************************/

		bool isOnTerrain();

		/*
			Inizializza i parametri per la caduta
		*/
		void startFall();

		/*
			Muove la posizione di un blocco in basso
		*/
		void fall();

		/*
			Imposta i parametri per interrompere la caduta
		*/
		void stopFall();

		/*
			Restituisce true se si può procedere nell'animazione della caduta. False altrimenti
		*/
		bool canFall();

		/* FINE GESTIONE CADUTA
		************************/

		void incCounters();
};