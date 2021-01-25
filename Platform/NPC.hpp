#pragma once
#include "Entity.hpp"
#include "settings.h"
#include "Weapon.hpp"

const bool NPC_HOSPITAL = true;
const bool NPC_WEAPONSHOP = false;

class NPC : public Entity {
	protected:
		struct WeaponShop {
			Weapon weapon;
			int price;
		};

		char name[STRING_LEN];
		bool type;

		// Mercante di armi
		WeaponShop shop[3];
		int curr_selected;
		int weapon_number;

		// Medico
		int price_hp;

	public:
		NPC(int health=0, Pixel head_left=Pixel(), Pixel head_right=Pixel(), Pixel body=Pixel(), Position position=Position(), bool type=false, int difficulty=0);

		bool getType();
		void getName(char name[]);

		/**********************
			Mercante di armi
		**********************/
		int getWeaponNumber();
		int getCurrSelected();
		/*
			Prende in input un intero.
			Imposta curr_selected a quel valore. Se supera i limiti, viene corretto il valore.
		*/
		void setCurrSelected(int curr_selected);
		/*
			Restituisce il campo weapon della locazione di shop indicata da curr_selected.
		*/
		Weapon getCurrWeapon();
		/*
			Restituisce il campo price della locazione di shop indicata da curr_selected.
		*/
		int getCurrWeaponPrice();
		/*
			Elimina la locazione di shop indicata da curr_selected.
		*/
		void deleteCurrentWeapon();


		/************
			Medico
		************/
		int getPriceHP();
};

