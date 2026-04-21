#ifndef __EXTRALIFEPOWERUP_H__
#define __EXTRALIFEPOWERUP_H__

#include "PowerUp.h"
#include "Player.h"

class ExtraLifePowerUp : public PowerUp {
public:
	ExtraLifePowerUp(Player* player);
	virtual ~ExtraLifePowerUp(void);

protected:
	void ApplyEffect(Spaceship& ship) override;
};

#endif