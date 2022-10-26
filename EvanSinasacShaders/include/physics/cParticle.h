#pragma once

//#include <extern_includes.h>
#include "../../GLCommon.h"
#include "../../GLMCommon.h"

namespace nPhysics
{
	class cParticle
	{
	private:
		// Linear position of the particle in world space
		glm::vec3 mPosition;
		// Linear velocity of the particle in world space
		glm::vec3 mVelocity;
		// Linear acceleration of the particle in world space
		glm::vec3 mAcceleration;
		// Inverse mass
		float mInverseMass;
		// Externally applied forces accumulated during a time step
		glm::vec3 mAppliedForce;
		// Damping is applied to linear motion
		// Required to remove energy added through numerical instability of integration
		// Also generally used as an overall "friction" for the particle
		float mDamping;

		float mRadius;

		//bool mIsAlive;
		// Not 100% sure why, but the cParticleWorld doesn't like my cBullet and cLaser, and causes the particles to bounce unexpectedly,
		// so we're only using cParticle, adding a timer which will be set whenever a Particle is made.  Also, a new bool to know if we're
		// using the timer, since I don't want the entities to die
		bool useTimer;
		float aliveTimer;

		// Going to use this to determine restitution for collisions
		// 0 - basic particle
		// 1 - target
		// 2 - Hanging Chain
		// 3 - Box Entity
		// 4 - Spinning Blade
		// 5 - target entity
		//int type;
	public:
		cParticle(float mass, const glm::vec3& position);
		virtual ~cParticle();

		cParticle() = delete;
		cParticle(cParticle& other) = delete;
		cParticle& operator=(cParticle& other) = delete;

		void SetIsAlive(bool isAlive);
		bool GetIsAlive() const;

		void SetDamping(float damping);
		float GetDamping() const;

		void SetMass(float mass);
		float GetMass() const;
		float GetInverseMass() const;

		bool IsStaticObject() const;

		glm::vec3 GetPosition() const;
		void GetPosition(glm::vec3& position);
		void SetPosition(const glm::vec3& position);

		glm::vec3 GetVelocity() const;
		void GetVelocity(glm::vec3& velocity);
		void SetVelocity(const glm::vec3& velocity);

		glm::vec3 GetAcceleration() const;
		void GetAcceleration(glm::vec3& acceleration);
		void SetAcceleration(const glm::vec3& acceleration);


		void ApplyForce(const glm::vec3& force);
		void ApplyImpulse(const glm::vec3& impulse);

		// Added for Project 3 (could've/should've made an inherited class that adds them)
		void SetRadius(float radius);
		float GetRadius();

		void ClearAppliedForces();

		virtual void Integrate(float deltaTime);

		void SetUseTimer(bool useIt);
		void SetTimer(float timeAlive);

		//void SetType(int ourType);
		//int GetType();

		bool mIsAlive;
	};
}