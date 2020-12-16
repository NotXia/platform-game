#pragma once
#include "ArmedEntity.hpp"
#include "Weapon.hpp"

class Enemy : public ArmedEntity {
	protected:
		int points;
		int money;
		Position *lastPlayerPosition;
		int visualRange;

	public:
		Enemy(int health=0, int points=0, int min_money=0, int max_money=0, Pixel head_left=Pixel(), Pixel head_right=Pixel(), Pixel body=Pixel(), Position position=Position(), Weapon *weapon=NULL, int visualRange=0);
};

