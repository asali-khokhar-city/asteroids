#include "PowerUp.h"
#include "BoundingSphere.h"
#include "Spaceship.h"
#include "Player.h"
#include "Logger.h"

PowerUp::PowerUp(Player* player)
	: GameObject("PowerUp"), mPlayer(player), mLogger("asteroid.log") {
}

PowerUp::~PowerUp(void)
{
}

bool PowerUp::CollisionTest(shared_ptr<GameObject> o) {
	// Only react to spaceships
	if (o->GetType() != GameObjectType("Spaceship")) return false;
	if (mBoundingShape.get() == NULL) return false;
	if (o->GetBoundingShape().get() == NULL) return false;
	return mBoundingShape->CollisionTest(o->GetBoundingShape());
}

void PowerUp::OnCollision(const GameObjectList& objects) {
	mLogger.debug("Testing for powerup collisions.");
	for (const auto& o : objects) {
		std::string typeName = o->GetType().GetTypeName();
		if (typeName == "Spaceship") {
			mLogger.debug("Powerup effect is applied from collision.");
			Spaceship* ship = dynamic_cast<Spaceship*>(o.get());
			ApplyEffect(*ship);

			mWorld->FlagForRemoval(GetThisPtr());
		}
	}
}