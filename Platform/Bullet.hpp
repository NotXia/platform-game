#pragma once
#include "Pixel.hpp"
#include "Position.hpp"

class Bullet {
	protected:
		Pixel texture;
		int damage;
		int speed;
		int range;

	public:
		Bullet(Pixel texture=Pixel(), int damage=1, int speed=1, int range=1);

		Pixel getTexture();
		int getDamage();
		int getSpeed();
		int getRange();
};