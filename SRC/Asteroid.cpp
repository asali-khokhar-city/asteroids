#include <stdlib.h>
#include "GameUtil.h"
#include "Asteroid.h"
#include "BoundingShape.h"
#include "BoundingSphere.h"
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
	mVelocity.y = 10.0 * sin(DEG2RAD * mAngle);
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
				ClampSpeed();
				other->ClampSpeed();
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

	if (dist == 0) { return; }

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
	float restitution = 1;
	float j = -(1 + restitution) * velocityAlongNormal / 2;

	GLVector3f vel1 = GetVelocity();
	GLVector3f vel2 = other.GetVelocity();

	SetVelocity(GLVector3f(vel1.x -= j * nx,
		vel1.y -= j * ny,
		vel1.z));
	SetVelocity(GLVector3f(vel2.x += j * nx,
		vel2.y += j * ny,
		vel2.z));

	/*
	float impX = nx * velocityAlongNormal / 2;
	float impY = ny * velocityAlongNormal / 2;

	SetVelocity(GLVector3f(GetVelocity().x + impX,
		GetVelocity().y + impY,
		GetVelocity().z));

	other.SetVelocity(GLVector3f(other.GetVelocity().x + impX,
		other.GetVelocity().y + impY,
		other.GetVelocity().z));
	*/

	// Find radius of asteroids
	shared_ptr<BoundingSphere> sphere =
		dynamic_pointer_cast<BoundingSphere>(GetBoundingShape());
	shared_ptr<BoundingSphere> otherSphere =
		dynamic_pointer_cast<BoundingSphere>(other.GetBoundingShape());

	float radius = 0;
	float otherRadius = 0;

	if (sphere && otherSphere) {
		radius = sphere->GetRadius();
		otherRadius = otherSphere->GetRadius();
	}

	// Calculate overlap
	float overlap = radius + otherRadius - dist;
	if (overlap > 0) {
		SetPosition(GLVector3f(vel1.x -= nx * overlap / 2,
			vel1.y -= ny * overlap / 2,
			vel1.z));
		SetPosition(GLVector3f(vel2.x += nx * overlap / 2,
			vel2.y += ny * overlap / 2,
			vel2.z));
		/*
		mPosition.x -= nx * overlap / 2.0f;
		mPosition.y -= ny * overlap / 2.0f;
		other.mPosition.x += nx * overlap / 2.0f;
		other.mPosition.y += ny * overlap / 2.0f;
		*/
	}
}

void Asteroid::ClampSpeed() {
	float speed = sqrt(mVelocity.x * mVelocity.x +
		mVelocity.y * mVelocity.y +
		mVelocity.z * mVelocity.z);

	if (speed > maxSpeed)
	{
		float scale = maxSpeed / speed;
		mVelocity.x *= scale;
		mVelocity.y *= scale;
		mVelocity.z *= scale; // This is always 0 anyways
	}
}