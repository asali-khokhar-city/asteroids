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
	GLVector3f pos1 = GetPosition();
	GLVector3f pos2 = other.GetPosition();

	// Normal vector
	GLVector3f normal = pos2 - pos1;
	// Find vector magnitude (length of vector)
	float dist = normal.length();
	// Don't divide by 0
	if (dist == 0.0f) return; 
	// Normalise vector
	normal /= dist;

	// Find relative velocity
	GLVector3f vel1 = GetVelocity();
	GLVector3f vel2 = other.GetVelocity();
	GLVector3f relVel = vel2 - vel1;

	// Find velocity along normal
	float velAlongNormal = relVel.dot(normal);
	// If they are not moving apart then ignore
	// (objects are already moving away from one another)
	if (velAlongNormal > 0) return; 

	// 0 to 1
	// More than 1 would increase velocity after collisions
	// 1 is perfect bounce
	float bounceCoefficient = 1.0f; 
	// Calculate impulse
	float impulseMag = -(1 + bounceCoefficient) * velAlongNormal / 2.0f;

	// Calculate impulse vector
	GLVector3f impulse = normal * impulseMag;

	// Apply velocities
	SetVelocity(vel1 - impulse);
	other.SetVelocity(vel2 + impulse);

	// Fix overlap
	auto sphere1 = dynamic_pointer_cast<BoundingSphere>(GetBoundingShape());
	auto sphere2 = dynamic_pointer_cast<BoundingSphere>(other.GetBoundingShape());

	// If they're not spheres something has gone wrong...
	if (!sphere1 || !sphere2) return;

	// Find overlap by looking at if the combined radii or larger than the distance between them
	float overlap = sphere1->GetRadius() + sphere2->GetRadius() - dist;

	// If there is an overlap, correct it by moving both asteroids away an equal distance
	// in opposite directions
	if (overlap > 0.0f) {
		GLVector3f correction = normal * (overlap / 2.0f);
		SetPosition(pos1 - correction);
		other.SetPosition(pos2 + correction);
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