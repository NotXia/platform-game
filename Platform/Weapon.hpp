#pragma once
#include "Pixel.hpp"
#include "Bullet.hpp"

class Weapon {
	protected:
		Pixel textureLeft;
		Pixel textureRight;
		Bullet bullet;

	public:
		Weapon(Pixel left, Pixel right, Bullet bullet);

		Pixel getTexture(bool direction);
};

