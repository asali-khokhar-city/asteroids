#ifndef __POWERUP_H__
#define __POWERUP_H__

#include "GameObject.h"
#include "Player.h"
#include "Spaceship.h"
#include "Logger.h"

class PowerUp : public GameObject {
public:
	PowerUp(Player* player);
	virtual ~PowerUp(void);

	bool CollisionTest(shared_ptr<GameObject> o);
	void OnCollision(const GameObjectList& objects);

protected:
	virtual void ApplyEffect(Spaceship& ship) = 0;
	Player* mPlayer;
	Logger mLogger;
};

#endif