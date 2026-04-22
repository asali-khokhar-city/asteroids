#ifndef __ASTEROID_H__
#define __ASTEROID_H__

#include "GameObject.h"
#include "Logger.h"

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

	AsteroidSize GetSize();
	bool DestroyedByBullet() { return mDestroyedByBullet; }
private:
	float mMaxSpeed = 10;
	bool mDestroyedByBullet = false;
	shared_ptr<Sprite> mSprite;

	AsteroidSize mSize;

	void BounceWith(GameObject& other);
	void Split(int count);

	static Logger mLogger;
};

#endif
