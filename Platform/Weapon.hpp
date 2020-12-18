#pragma once
#include "Pixel.hpp"
#include "Bullet.hpp"

class Weapon {
	protected:
		Pixel textureLeft;
		Pixel textureRight;
		Bullet bullet;
		int range;

	public:
		Weapon(Pixel left, Pixel right, Bullet bullet, int range);

		Pixel getTexture(bool direction);

		int getRange();
};

