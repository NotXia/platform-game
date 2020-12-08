#include "Pixel.hpp"

class Weapon {
	protected:
		int range;
		int damage;
		Pixel textureLeft;
		Pixel textureRight;

	public:
		Weapon(int range, int damage, Pixel left, Pixel right);
};

