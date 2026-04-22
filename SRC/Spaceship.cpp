#include "GameUtil.h"
#include "GameWorld.h"
#include "Bullet.h"
#include "Spaceship.h"
#include "BoundingSphere.h"
#include "Asteroid.h"
#include "Logger.h"
#include <algorithm>

Logger Spaceship::mLogger("spaceship.log");

using namespace std;

// PUBLIC INSTANCE CONSTRUCTORS ///////////////////////////////////////////////

/**  Default constructor. */
Spaceship::Spaceship()
	: GameObject("Spaceship"), mThrust(0)
{
	SetMaxSpeed(20);
}

/** Construct a spaceship with given position, velocity, acceleration, angle, and rotation. */
Spaceship::Spaceship(GLVector3f p, GLVector3f v, GLVector3f a, GLfloat h, GLfloat r)
	: GameObject("Spaceship", p, v, a, h, r), mThrust(0)
{
	SetMaxSpeed(20);
}

/** Copy constructor. */
Spaceship::Spaceship(const Spaceship& s)
	: GameObject(s), mThrust(0)
{
	SetMaxSpeed(20);
}

/** Destructor. */
Spaceship::~Spaceship(void)
{
}

// PUBLIC INSTANCE METHODS ////////////////////////////////////////////////////

/** Update this spaceship. */
void Spaceship::Update(int t)
{
	// Check/update invulnerability
	CheckInvuln(t);

	// Call parent update function
	GameObject::Update(t);
}

/** Render this spaceship. */
void Spaceship::Render(void)
{
	if (mSpaceshipShape.get() != NULL) mSpaceshipShape->Render();

	// If ship is thrusting
	if ((mThrust > 0) && (mThrusterShape.get() != NULL)) {
		mThrusterShape->Render();
	}

	GameObject::Render();

	// If ship is invulnerable
	if (mInvuln && mShieldShape.get() != NULL) {
		glLineWidth(3.0f);
		mLogger.debug("Shield is attempting to be rendered.");
		mShieldShape->Render();
		glLineWidth(1.0f);
	}
}

/** Fire the rockets. */
void Spaceship::Thrust(float t)
{
	mThrust = t;
	// Increase acceleration in the direction of ship
	mAcceleration.x = mThrust*cos(DEG2RAD*mAngle);
	mAcceleration.y = mThrust*sin(DEG2RAD*mAngle);
}

/** Cool the jets. */
void Spaceship::Brake(float factor)
{
	// Ensure that factor is between 0 and 1
	factor = std::clamp(factor, 0.0f, 1.0f);
	mVelocity *= factor;
}

/** Set the rotation. */
void Spaceship::Rotate(float r)
{
	mRotation = r;
}

/** Shoot a bullet. */
void Spaceship::Shoot(void)
{
	// Check the world exists
	if (!mWorld) return;
	// Construct a unit length vector in the direction the spaceship is headed
	GLVector3f spaceship_heading(cos(DEG2RAD*mAngle), sin(DEG2RAD*mAngle), 0);
	spaceship_heading.normalize();
	// Calculate the point at the node of the spaceship from position and heading
	GLVector3f bullet_position = mPosition + (spaceship_heading * 4);
	// Calculate how fast the bullet should travel
	float bullet_speed = 30;
	// Construct a vector for the bullet's velocity
	GLVector3f bullet_velocity = mVelocity + spaceship_heading * bullet_speed;
	// Construct a new bullet
	shared_ptr<GameObject> bullet
		(new Bullet(bullet_position, bullet_velocity, mAcceleration, mAngle, 0, 2000));
	bullet->SetBoundingShape(make_shared<BoundingSphere>(bullet->GetThisPtr(), 2.0f));
	bullet->SetShape(mBulletShape);
	// Add the new bullet to the game world
	mWorld->AddObject(bullet);

}

bool Spaceship::CollisionTest(shared_ptr<GameObject> o)
{
	if (o->GetType() != GameObjectType("Asteroid")) return false;
	if (mBoundingShape.get() == NULL) return false;
	if (o->GetBoundingShape().get() == NULL) return false;
	return mBoundingShape->CollisionTest(o->GetBoundingShape());
}

void Spaceship::OnCollision(const GameObjectList &objects)
{
	bool removed = false;
	for (const auto& o : objects) {
		std::string typeName = o->GetType().GetTypeName();
		if (typeName == "Asteroid") {
			// Cast to Asteroid
			if (Asteroid* asteroid = dynamic_cast<Asteroid*>(o.get())) {
				if (asteroid->GetSize() == Asteroid::AsteroidSize::SMALL) {
					mLogger.debug("Spaceship has collided with small asteroid.");

				}
				else {
					if (!removed && !mInvuln) {
						removed = true;
						mWorld->FlagForRemoval(GetThisPtr());
						mLogger.debug("Spaceship has collided with large asteroid.");
					}
					else {
						mLogger.debug("Spaceship has ignored collision with large asteroid.");
					}
				}
			}
		}
	}
}

float Spaceship::GetMaxSpeed() {
	return mMaxSpeed;
}

void Spaceship::ActivateInvulnerability(int duration) {
	mLogger.debug("Spaceship has become invulnerable.");
	mInvuln = true;
	mInvulnRemaining = duration;
}

void Spaceship::CheckInvuln(int t) {
	if (mInvuln)
	{
		mLogger.debug("Spaceship is currently invulnerable.");
		mInvulnRemaining -= t;
		if (mInvulnRemaining <= 0) {
			mLogger.debug("Spaceship is no longer invulnerable.");
			mInvuln = false;
		}
	}
}