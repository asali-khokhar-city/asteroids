#ifndef __SPACESHIP_H__
#define __SPACESHIP_H__

#include "GameUtil.h"
#include "GameObject.h"
#include "Shape.h"
#include "Logger.h"

class Spaceship : public GameObject
{
public:
	Spaceship();
	Spaceship(GLVector3f p, GLVector3f v, GLVector3f a, GLfloat h, GLfloat r);
	Spaceship(const Spaceship& s);
	virtual ~Spaceship(void);

	virtual void Update(int t);
	virtual void Render(void);

	virtual void CalculateMovement();
	virtual void Thrust(bool t);
	virtual void Brake(bool b);
	virtual void Dash();
	virtual void Rotate(float r);
	virtual void Shoot(void);

	void SetSpaceshipShape(shared_ptr<Shape> spaceship_shape) { mSpaceshipShape = spaceship_shape; }
	void SetThrusterShape(shared_ptr<Shape> thruster_shape) { mThrusterShape = thruster_shape; }
	void SetBulletShape(shared_ptr<Shape> bullet_shape) { mBulletShape = bullet_shape; }
	void setShieldShape(shared_ptr<Shape> shield_shape) { mShieldShape = shield_shape; }

	bool CollisionTest(shared_ptr<GameObject> o);
	void OnCollision(const GameObjectList &objects);

	float GetMaxSpeed();

	void ActivateInvulnerability(int duration);

private:
	const float THRUST_POWER = 10.0f;
	const float BRAKE_FACTOR = 0.98f;
	const int DASH_TIME = 200;
	const int DASH_COOLDOWN_TIME = 5000;
	const float DASH_POWER = 80.0f;

	float mThrust;
	bool mThrusting;
	bool mBraking;
	int mInvulnRemaining;
	bool mInvuln = false;
	int mDashRemaining;
	int mDashCooldown;
	bool mDashing = false;

	shared_ptr<Shape> mSpaceshipShape;
	shared_ptr<Shape> mThrusterShape;
	shared_ptr<Shape> mBulletShape;
	shared_ptr<Shape> mShieldShape;

	static Logger mLogger;

	void InvulnTimer(int t);
	void DashCooldownTimer(int t);
	void DashTimer(int t);
};

#endif