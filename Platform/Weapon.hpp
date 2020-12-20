#pragma once
#include "Pixel.hpp"
#include "Bullet.hpp"

class Weapon {
	protected:
		Pixel textureLeft;
		Pixel textureRight;
		Bullet bullet;

	public:
		Weapon(Pixel left=Pixel(), Pixel right=Pixel(), Bullet bullet=Bullet());

		Pixel getTexture(bool direction);

		Bullet getBullet();
};

