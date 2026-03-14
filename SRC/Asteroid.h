#ifndef __ASTEROID_H__
#define __ASTEROID_H__

#include "GameObject.h"
#include "Logger.h"

class Asteroid : public GameObject
{
public:
	Asteroid(void);
	~Asteroid(void);

	bool CollisionTest(shared_ptr<GameObject> o);
	void OnCollision(const GameObjectList& objects);
private:
	float maxSpeed = 10;
	void BounceWith(Asteroid& other);
	void ClampSpeed();
	static Logger logger;
};

#endif
