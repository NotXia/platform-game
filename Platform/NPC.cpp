#include "NPC.hpp"
#include <cstring>
#include <cstdlib>
#include "WeaponContainer.hpp"

NPC::NPC(int health, Pixel head_left, Pixel head_right, Pixel body, Position position, bool type, int difficulty) : Entity(health, head_left, head_right, body, position) {
	char names[][STRING_LEN] = { "Alfredo", "Ilenia", "Federigo", "Antonia", "Damiano", "Serena" };
	int names_size = 6;
	this->price_hp = 0;
	this->curr_selected = 0;
	this->weapon_number = 0;
	
	strncpy(this->name, names[rand()%names_size], STRING_LEN);

	this->type = type;
	if (type == NPC_HOSPITAL) {
		this->price_hp = difficulty;
	}
	else {
		WeaponContainer *weaponcontainer = new WeaponContainer();
		weaponcontainer->initForPlayer();

		Weapon weapon;
		int price;
		int tier2_chance, tier3_chance, tierS_chance;
		bool found;
		for (int i=0; i<3; i++) {
			found = false;
			tier2_chance = 10 + (difficulty+1) * 40;
			tier3_chance = (difficulty+1) * 45;
			tierS_chance = difficulty;

			int generate = rand() % (tier2_chance+tier3_chance+tierS_chance);
			if (generate < tier2_chance) {
				weapon = weaponcontainer->getRandomTier2();
				price = 10 * difficulty;
			}
			else if (generate < tier2_chance+tier3_chance) {
				weapon = weaponcontainer->getRandomTier3();
				price = 20 * difficulty;
			}
			else {
				weapon = weaponcontainer->getRandomSpecial();
				price = 50 * difficulty;
			}

			int j=0; 
			while (j < weapon_number && !found) {
				if (shop[j].weapon.equals(weapon)) {
					found = true;
				}
				j++;
			}

			if (!found) {
				shop[weapon_number].weapon = weapon;
				shop[weapon_number].price = price;
				weapon_number++;
			}
		}

		delete weaponcontainer;
	}
}


bool NPC::getType() {
	return type;
}

void NPC::getName(char name[]) {
	strncpy(name, this->name, STRING_LEN);
}

/**********************
	Mercante di armi
**********************/
int NPC::getCurrSelected() {
	return curr_selected;
}

int NPC::getWeaponNumber() {
	return weapon_number;
}

/*
	Prende in input un intero.
	Imposta curr_selected a quel valore. Se supera i limiti, viene corretto il valore
*/
void NPC::setCurrSelected(int curr_selected) {
	this->curr_selected = curr_selected;
	if (this->curr_selected >= weapon_number) {
		this->curr_selected = 0;
	}
	else if(this->curr_selected < 0) {
		this->curr_selected = weapon_number-1;
	}
}

/*
	Restituisce il campo weapon della locazione di shop indicata da curr_selected
*/
Weapon NPC::getCurrWeapon() {
	return shop[curr_selected].weapon;
}

/*
	Restituisce il campo price della locazione di shop indicata da curr_selected
*/
int NPC::getCurrWeaponPrice() {
	return shop[curr_selected].price;
}

/*
	Elimina la locazione di shop indicata da curr_selected
*/
void NPC::deleteCurrentWeapon() {
	for (int i=curr_selected; i<weapon_number-1; i++) {
		shop[i] = shop[i+1];
	}
	weapon_number--;
	setCurrSelected(curr_selected);
}

/************
	Medico
************/
int NPC::getPriceHP() {
	return price_hp;
}