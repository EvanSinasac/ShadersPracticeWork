#include <physics/cParticle.h>

namespace nPhysics
{
	cParticle::cParticle(float mass, const glm::vec3& position)
		: mPosition(position)
		, mVelocity(0.f)
		, mAcceleration(0.f)
		, mDamping(0.995f)
		, mAppliedForce(0.f)
		, mIsAlive(true)
		, mRadius (1.0f)
	{
		SetMass(mass);
		this->SetUseTimer(false);
		this->SetTimer(0.0f);
		// TODO: Make setters/getters and use this to affect restitution in contacts
		//this->type = 0;
	}
	cParticle::~cParticle()
	{

	}

	void cParticle::SetDamping(float damping)
	{
		mDamping = glm::clamp(damping, 0.f, 1.f);
	}

	float cParticle::GetDamping() const
	{
		return mDamping;
	}

	void cParticle::SetIsAlive(bool isAlive)
	{
		mIsAlive = isAlive;
	}

	bool cParticle::GetIsAlive() const
	{
		return mIsAlive;
	}

	void cParticle::SetMass(float mass)
	{
		if (mass <= 0.f)
		{
			mInverseMass = 0.f;
		}
		else
		{
			mInverseMass = 1.f / mass;
		}
	}

	float cParticle::GetMass() const
	{
		if (mInverseMass == 0.f)
		{
			return mInverseMass;
		}
		return 1.0f / mInverseMass;
	}

	float cParticle::GetInverseMass() const
	{
		return mInverseMass;
	}

	bool cParticle::IsStaticObject() const
	{
		return mInverseMass == 0.f;
	}

	glm::vec3 cParticle::GetPosition() const
	{
		return mPosition;
	}

	void cParticle::GetPosition(glm::vec3& position)
	{
		position = mPosition;
	}

	void cParticle::SetPosition(const glm::vec3& position)
	{
		mPosition = position;
	}

	glm::vec3 cParticle::GetVelocity() const
	{
		return mVelocity;
	}

	void cParticle::GetVelocity(glm::vec3& velocity)
	{
		velocity = mVelocity;
	}

	void cParticle::SetVelocity(const glm::vec3& velocity)
	{
		mVelocity = velocity;
	}

	glm::vec3 cParticle::GetAcceleration() const
	{
		return mAcceleration;
	}

	void cParticle::GetAcceleration(glm::vec3& acceleration)
	{
		acceleration = mAcceleration;
	}

	void cParticle::SetAcceleration(const glm::vec3& acceleration)
	{
		mAcceleration = acceleration;
	}

	void cParticle::ApplyForce(const glm::vec3& force)
	{
		mAppliedForce += force;
	}

	void cParticle::ApplyImpulse(const glm::vec3& impulse)
	{
		mVelocity += impulse * mInverseMass;
	}

	void cParticle::SetRadius(float radius)
	{
		mRadius = radius;
	}
	float cParticle::GetRadius()
	{
		return mRadius;
	}

	void cParticle::ClearAppliedForces()
	{
		glm::set(mAppliedForce, 0.f, 0.f, 0.f);
	}

	void cParticle::Integrate(float deltaTime)
	{
		if (mInverseMass == 0.f)
		{
			return; // static things don't move!
		}

		// Integrating with Euler algorithm
		mPosition += mVelocity * deltaTime;
		// F*(1/m) = a
		mVelocity += (mAcceleration + mAppliedForce * mInverseMass) * deltaTime;

		// Apply damping
		mVelocity *= glm::pow(mDamping, deltaTime);

		if (this->useTimer)
		{
			this->aliveTimer -= deltaTime;
			if (this->aliveTimer <= 0.0f)
			{
				this->SetIsAlive(false);
			}
		}

		/*if (mPosition.y <= 0.0f)
		{
			mIsAlive = false;
		}*/

		// Clear applied forces
		ClearAppliedForces();
	}

	void cParticle::SetUseTimer(bool useIt)
	{
		this->useTimer = useIt;
	}
	void cParticle::SetTimer(float timeAlive)
	{
		this->aliveTimer = timeAlive;
	}

	//void cParticle::SetType(int ourType)
	//{
	//	this->type = ourType;
	//}
	//int cParticle::GetType()
	//{
	//	return this->type;
	//}
}