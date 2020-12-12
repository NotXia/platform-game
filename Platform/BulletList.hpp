#include "Bullet.hpp"
#include "Position.hpp"

class BulletList {
	protected:
		struct FlyingBullet {
			Bullet bullet;
			Position position;
			bool hostile;
			int distance;
			FlyingBullet *next;
		};

		FlyingBullet *list;

	public:
		BulletList();

		void insert(Bullet bullet, Position position, bool hostile);

		
};

