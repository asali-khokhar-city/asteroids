#include "InvulnerablePowerUp.h"
#include "Spaceship.h"
#include "Player.h"

InvulnerablePowerUp::InvulnerablePowerUp(Player* player) :
	PowerUp(player)
{
	mType = GameObjectType("InvulnerablePowerUp");
	mDuration = 5000;
}

InvulnerablePowerUp::~InvulnerablePowerUp(void)
{
}

void InvulnerablePowerUp::ApplyEffect(Spaceship& ship)
{
	ship.ActivateInvulnerability(mDuration);
}