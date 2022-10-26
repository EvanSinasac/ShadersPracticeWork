#include <physics/cParticleContact.h>


namespace nPhysics
{

	void cParticleContact::Resolve(float deltaTime)
	{
		ResolveVelocity(deltaTime);
		ResolvePosition(deltaTime);
	}

	float cParticleContact::CalculateSeparatingVelocity() const
	{
		glm::vec3 relativeVelocity = mParticle0->GetVelocity();
		if (mParticle1)
		{
			relativeVelocity -= mParticle1->GetVelocity();
		}
		return glm::dot(relativeVelocity, mContactNormal);
	}

	void cParticleContact::ResolveVelocity(float deltaTime)
	{
		// Find the velocity in the direction of the contact
		float separatingVelocity = CalculateSeparatingVelocity();

		if (separatingVelocity > 0.0f)
		{
			// The contact is either already separating, or stationary
			// no impulse is required
			return;
		}

		// calculate the new separating velocity
		float newSepVelocity = -separatingVelocity * mRestitution;

		// Check the velocity build-up due to accelaration only
		glm::vec3 accCausedVelocity = mParticle0->GetAcceleration();
		if (mParticle1)
		{
			accCausedVelocity -= mParticle1->GetAcceleration();
		}
		float accCausedSepVelocity = glm::dot(accCausedVelocity, mContactNormal) * deltaTime;

		// If we've got a closing velocity due to acceleration build-up
		// remove it from the new separating velocity
		if (accCausedSepVelocity < 0)
		{
			newSepVelocity += mRestitution * accCausedSepVelocity;

			// Make sure we haven't removed more than there was to remove.
			if (newSepVelocity < 0.0f)
			{
				newSepVelocity = 0.0f;
			}
		}

		float deltaVelocity = newSepVelocity - separatingVelocity;


		// We apply the change in velocity to each object in proportion to their
		// inverse mass (higher mass -> get less change in velocity)
		float totalInverseMass = mParticle0->GetInverseMass();
		if (mParticle1)
		{
			totalInverseMass += mParticle1->GetInverseMass();
		}

		// If all particles have infinite mass, then impulses have no effect
		if (totalInverseMass <= 0.0f)
		{
			return;
		}

		// Calculate the impulse to apply
		float impulse = deltaVelocity / totalInverseMass;

		// Find the amount of impulse per unit of inverse mass
		glm::vec3 impulsePerIMass = mContactNormal * impulse;

		// Apply impulses; The are applied in the direction of the contact
		// and they are proportional to the inverse mass
		mParticle0->SetVelocity(mParticle0->GetVelocity() + impulsePerIMass * mParticle0->GetInverseMass());

		if (mParticle1)
		{
			// Particle 1 will go in the opposite direction
			mParticle1->SetVelocity(mParticle1->GetVelocity() + impulsePerIMass * -mParticle1->GetInverseMass());
		}
	}

	void cParticleContact::ResolvePosition(float deltaTime)
	{
		// If we don't have any penetration, skip this step
		if (mPenetration <= 0)
		{
			return;
		}

		// The movement of each object is based on their inverse mass,
		// so get the total
		float totalInverseMass = mParticle0->GetInverseMass();
		if (mParticle1)
		{
			totalInverseMass += mParticle1->GetInverseMass();
		}

		// If all particles have infinite mass, then we do nothing.
		if (totalInverseMass <= 0)
		{
			glm::set(mMovement0, 0.0f, 0.0f, 0.0f);		//this is from James Lucas's personal glm stuff
			glm::set(mMovement1, 0.0f, 0.0f, 0.0f);
			return;
		}

		// Find the amount of mPenetration resolution per unit of inverse mass
		glm::vec3 movePerIMass = mContactNormal * (mPenetration / totalInverseMass);

		// Calculate the movement amounts
		mMovement0 = movePerIMass * mParticle0->GetInverseMass();
		if (mParticle1)
		{
			mMovement1 = movePerIMass * -mParticle1->GetInverseMass();
		}
		else
		{
			glm::set(mMovement1, 0.0f, 0.0f, 0.0f);
		}

		// Apply the penetration resolution
		mParticle0->SetPosition(mParticle0->GetPosition() + mMovement0);
		if (mParticle1)
		{
			mParticle1->SetPosition(mParticle1->GetPosition() + mMovement1);
		}
	}
}