#ifndef __INVULNERABLEPOWERUP_H__
#define __INVULNERABLEPOWERUP_H__

#include "PowerUp.h"
#include "Player.h"

class InvulnerablePowerUp : public PowerUp {
public:
	InvulnerablePowerUp(Player* player);
	virtual ~InvulnerablePowerUp(void);

protected:
	void ApplyEffect(Spaceship& ship) override;
	
private:
	int mDuration = 5000;
};

#endif