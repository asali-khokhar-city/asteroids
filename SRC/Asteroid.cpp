#include <stdlib.h>
#include "GameUtil.h"
#include "Asteroid.h"
#include "BoundingShape.h"
#include <string>

Logger Asteroid::logger("asteroid.log");

Asteroid::Asteroid(void) : GameObject("Asteroid")
{
	mAngle = rand() % 360;
	mRotation = 0; // rand() % 90;
	mPosition.x = rand() / 2;
	mPosition.y = rand() / 2;
	mPosition.z = 0.0;
	mVelocity.x = 10.0 * cos(DEG2RAD*mAngle);
	mVelocity.y = 10.0 * sin(DEG2RAD*mAngle);
	mVelocity.z = 0.0;
}

Asteroid::~Asteroid(void)
{
}

bool Asteroid::CollisionTest(shared_ptr<GameObject> o)
{
	// if (GetType() == o->GetType()) return false;
	if (mBoundingShape.get() == NULL) return false;
	if (o->GetBoundingShape().get() == NULL) return false;
	return mBoundingShape->CollisionTest(o->GetBoundingShape());
}

void Asteroid::OnCollision(const GameObjectList& objects)
{
	for (const auto &o : objects) {
		std::string typeName = o->GetType().GetTypeName();
		logger.debug("Checking collision with object of type: " + typeName + ".");
		if (typeName == "Asteroid") {
			logger.debug("Asteroid has collided with another asteroid.");

			// Cast to Asteroid
			Asteroid* other = dynamic_cast<Asteroid*>(o.get());
			if (other) {
				BounceWith(*other);
			}
		}
		else {
			mWorld->FlagForRemoval(GetThisPtr());
		}
	}
}

void Asteroid::BounceWith(Asteroid& other) {
	// Calculate collision vector between asteroids
	float nx = other.GetPosition().x - GetPosition().x;
	float ny = other.GetPosition().y - GetPosition().y;
	float dist = sqrt(nx * nx + ny * ny);

	// Normalise collision vector
	nx /= dist;
	ny /= dist;

	// Find relative velocity along normal
	float nvx = other.GetVelocity().x - GetVelocity().x;
	float nyx = other.GetVelocity().y - GetVelocity().y;

	float velocityAlongNormal = nvx * nx + nyx * ny;

	// Bounce if moving towards
	if (velocityAlongNormal > 0) return;

	// Swap velocities
	float impX = nx * velocityAlongNormal / 2;
	float impY = ny * velocityAlongNormal / 2;

	SetVelocity(GLVector3f(GetVelocity().x + impX,
		GetVelocity().y + impY,
		GetVelocity().z));

	other.SetVelocity(GLVector3f(other.GetVelocity().x + impX,
		other.GetVelocity().y + impY,
		other.GetVelocity().z));
}
