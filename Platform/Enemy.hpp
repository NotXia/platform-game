#include "Entity.hpp"
#include "Weapon.hpp"

class Enemy : public Entity {
	protected:
		Weapon *weapon;
		int points;
		int money;
		Position *lastPlayerPosition;
		int visualRange;

		int weapon_loop_counter;

	public:
		Enemy(int health, int points, int min_money, int max_money, Pixel head_left, Pixel head_right, Pixel body, Weapon *weapon, Position position, int visualRange);
};

