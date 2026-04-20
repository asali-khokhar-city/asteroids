#ifndef __POWERUP_H__
#define __POWERUP_H__

#include "GameObject.h"
#include "Spaceship.h"

class PowerUp : public GameObject {
public:
	PowerUp();
	virtual ~PowerUp(void);

	bool CollisionTest(shared_ptr<GameObject> o);
	void OnCollision(const GameObjectList& objects);

protected:
	virtual void ApplyEffect(Spaceship& ship) = 0;
};

#endif