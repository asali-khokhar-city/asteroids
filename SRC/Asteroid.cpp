#include <stdlib.h>
#include "GameUtil.h"
#include "Asteroid.h"
#include "BoundingShape.h"
#include "BoundingSphere.h"
#include "Animation.h"
#include "AnimationManager.h"
#include <string>

Logger Asteroid::mLogger("asteroid.log");

// Construct without specifying size

Asteroid::Asteroid(AsteroidSize size) : GameObject("Asteroid")
{
	mSize = size;

	mAngle = rand() % 360;
	mRotation = 0; // rand() % 90;

	mPosition.x = rand() / 2;
	mPosition.y = rand() / 2;
	mPosition.z = 0.0;

	// Ternary to decide initial speed of asteroid
	// Smaller asteroids are faster
	float initialSpeed = (mSize == AsteroidSize::BIG) ? 10 : 15;

	mVelocity.x = initialSpeed * cos(DEG2RAD*mAngle);
	mVelocity.y = initialSpeed * sin(DEG2RAD * mAngle);
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
		mLogger.debug("Checking collision with object of type: " + typeName + ".");
		if (typeName == "Asteroid") {
			mLogger.debug("Asteroid has collided with another asteroid.");

			// Cast to Asteroid
			Asteroid* other = dynamic_cast<Asteroid*>(o.get());
			if (other) {
				BounceWith(*other);
				ClampSpeed();
				other->ClampSpeed();
			}
		}
		else if (typeName == "Bullet") {
			// spawn 2 smaller asteroids that do not damage the player
			mLogger.debug("Asteroid is struck by bullet. Splitting asteroid.");
			if (mSize == AsteroidSize::BIG) {
				Split(2);
			}

			mWorld->FlagForRemoval(GetThisPtr());
			break;
		}
		else {
			mWorld->FlagForRemoval(GetThisPtr());
		}
	}
}

Asteroid::AsteroidSize Asteroid::GetSize() {
	return mSize;
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

	if (speed > mMaxSpeed)
	{
		float scale = mMaxSpeed / speed;
		mVelocity.x *= scale;
		mVelocity.y *= scale;
		mVelocity.z *= scale; // This is always 0 anyways
	}
}

void Asteroid::Split(int count) {
	// Only big asteroids should be split
	if (mSize != AsteroidSize::BIG) {
		return;
	}

	// Size coefficient decides how much smaller the asteroid is
	float sizeCoefficient = 0.2;

	// Animation manager for asteroid sprite
	// Preparing sprite for being assigned
	Animation* anim_ptr = AnimationManager::GetInstance().GetAnimationByName("asteroid1");

	shared_ptr<Sprite> asteroid_sprite =
		make_shared<Sprite>(anim_ptr->GetWidth(), anim_ptr->GetHeight(), anim_ptr);

	asteroid_sprite->SetLoopAnimation(true);

	// Calculate new scale for sprite
	float scale = GetScale();
	float newScale = sizeCoefficient * scale;

	// Calculate new radius for collision
	auto sphere = dynamic_pointer_cast<BoundingSphere>(GetBoundingShape());
	float radius = sphere->GetRadius();
	float newRadius = sizeCoefficient * radius;

	// Add asteroids
	for (int i = 0; i < count; i++) {
		// Create new, small asteroid
		shared_ptr<Asteroid> newAsteroid = make_shared<Asteroid>(AsteroidSize::SMALL);
		// Set position at the origin of the original asteroid
		newAsteroid->SetPosition(GetPosition());
		// Set new scaled down visual size of the asteroid
		newAsteroid->SetScale(newScale);
		// Set new scaled down radius
		newAsteroid->SetBoundingShape(
			make_shared<BoundingSphere>(newAsteroid->GetThisPtr(), newRadius)
		);
		// Assign sprite
		newAsteroid->SetSprite(asteroid_sprite);
		// Add it to the game world
		mWorld->AddObject(newAsteroid);
	}
}