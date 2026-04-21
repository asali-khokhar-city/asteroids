#include "ExtraLifePowerUp.h"
#include "Spaceship.h"
#include "GameWorld.h"
#include "Asteroids.h"

ExtraLifePowerUp::ExtraLifePowerUp(Player* player)
	: PowerUp(player)
{
	mType = GameObjectType("ExtraLifePowerUp");
}

ExtraLifePowerUp::~ExtraLifePowerUp(void)
{
}

void ExtraLifePowerUp::ApplyEffect(Spaceship& ship)
{
	if (mPlayer) {
		mPlayer->AddLife();
	}
}

