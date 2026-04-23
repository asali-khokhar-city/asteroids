#ifndef __ASTEROID_H__
#define __ASTEROID_H__

#include "GameObject.h"
#include "Logger.h"
#include <vector>

class Asteroid : public GameObject
{
public:
	// Asteroid sizes
	enum class AsteroidSize {
		SMALL,
		BIG
	};

	Asteroid(AsteroidSize size = AsteroidSize::BIG);
	~Asteroid(void);

	bool CollisionTest(shared_ptr<GameObject> o);
	void OnCollision(const GameObjectList& objects);

	std::vector<shared_ptr<Asteroid>> Split(int count);

	AsteroidSize GetSize();
	bool DestroyedByBullet() { return mDestroyedByBullet; }
private:
	bool mDestroyedByBullet = false;
	float mMaxSpeed = 10;
	shared_ptr<Sprite> mSprite;

	AsteroidSize mSize;

	void BounceWith(GameObject& other);

	static Logger mLogger;
};

#endif
