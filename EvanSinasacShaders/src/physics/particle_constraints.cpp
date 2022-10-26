#include <physics/particle_constraints.h>


namespace nPhysics
{
	cParticleConstraint::cParticleConstraint(cParticle* particleA, cParticle* particleB)
		: mParticleA(particleA)
		, mParticleB(particleB)
	{

	}

	cParticleConstraint::~cParticleConstraint() {}

	float cParticleConstraint::CurrentSeparation() const
	{
		return glm::distance(mParticleA->GetPosition(), mParticleB->GetPosition());
	}

	cParticle* cParticleConstraint::GetParticleA()
	{
		return mParticleA;
	}

	cParticle* cParticleConstraint::GetParticleB()
	{
		return mParticleB;
	}

	cParticleRodConstraint::cParticleRodConstraint(cParticle* particleA, cParticle* particleB)
		: cParticleConstraint(particleA, particleB)
	{
		mLength = CurrentSeparation();
	}

	size_t cParticleRodConstraint::AddContact(cParticleContact* contact, size_t limit, std::vector<cParticle*>& particles) const
	{
		// find the current length of the rod
		float currentLen = CurrentSeparation();

		// Check if we're over-extended
		if (currentLen == mLength)
		{
			return 0;
		}

		// Otherwise return the contact
		contact->mParticle0 = mParticleA;
		contact->mParticle1 = mParticleB;

		// Calculate the normal
		glm::vec3 normal = glm::normalize(mParticleB->GetPosition() - mParticleA->GetPosition());

		// The contact normal depends on if we are extending or compressing
		if (currentLen > mLength)
		{
			contact->mContactNormal = normal;
			contact->mPenetration = currentLen - mLength;
		}
		else
		{
			contact->mContactNormal = normal * -1.0f;
			contact->mPenetration = mLength - currentLen;
		}

		// zero restitution = no bouncinesss.
		contact->mRestitution = 0.0f;

		return 1;
	}


	// Rope Constraint
	cParticleRopeConstraint::cParticleRopeConstraint(cParticle* particleA, cParticle* particleB)
		: cParticleConstraint(particleA, particleB)
	{
		mLength = CurrentSeparation();
	}

	size_t cParticleRopeConstraint::AddContact(cParticleContact* contact, size_t limit, std::vector<cParticle*>& particles) const
	{
		// find the current separation
		float currentLength = CurrentSeparation();

		// For the rope, the current separation can be less than or equal to the maximum distance
		if (currentLength <= mLength)
		{
			return 0;
		}
		// Otherwise, we have to bring the particle back
		// Otherwise return the contact
		contact->mParticle0 = mParticleA;
		contact->mParticle1 = mParticleB;

		// Calculate the normal
		glm::vec3 normal = glm::normalize(mParticleB->GetPosition() - mParticleA->GetPosition());

		// The contact normal depends on if we are extending or compressing
		// No contact if the distance is shorter (it's a rope, it can be smaller than the max length)
		//if (currentLength > mLength)
		//{
			contact->mContactNormal = normal;
			contact->mPenetration = currentLength - mLength;
		//}
		//else
		//{
		//	contact->mContactNormal = normal * -1.0f;
		//	contact->mPenetration = mLength - currentLen;
		//}

		// zero restitution = no bouncinesss.
		// some bounciness, it's a rope
		contact->mRestitution = 0.5f;

		return 1;
	}

}