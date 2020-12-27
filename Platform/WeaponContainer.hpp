#pragma once
#include "Weapon.hpp"

class WeaponContainer {
	protected:
		Weapon tier1[50]; int tier1_size;
		Weapon tier2[50]; int tier2_size;
		Weapon tier3[50]; int tier3_size;
		Weapon special[50]; int special_size;

		/*
			Prende in input un oggetto Weapon.
			Inserisce l'oggetto nell'array tier1.
		*/
		void insert_tier1(Weapon weapon);

		/*
			Prende in input un oggetto Weapon.
			Inserisce l'oggetto nell'array tier2.
		*/
		void insert_tier2(Weapon weapon);

		/*
			Prende in input un oggetto Weapon.
			Inserisce l'oggetto nell'array tier3.
		*/
		void insert_tier3(Weapon weapon);

		/*
			Prende in input un oggetto Weapon.
			Inserisce l'oggetto nell'array special.
		*/
		void insert_special(Weapon weapon);

	public:
		WeaponContainer();

		/*
			Inizializza gli array con le armi destinate al giocatore
		*/
		void initForPlayer();
		/*
			Inizializza gli array con le armi destinate ai nemici
		*/
		void initForEnemy();

		/*
			Restituisce un oggetto Weapon casuale contenuto nell'array tier1
		*/
		Weapon getRandomTier1();

		/*
			Restituisce un oggetto Weapon casuale contenuto nell'array tier2
		*/
		Weapon getRandomTier2();

		/*
			Restituisce un oggetto Weapon casuale contenuto nell'array tier3
		*/
		Weapon getRandomTier3();

		/*
			Restituisce un oggetto Weapon casuale contenuto nell'array special
		*/
		Weapon getRandomSpecial();
};

