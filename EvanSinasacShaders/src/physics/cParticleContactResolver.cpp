#include <physics/cParticleContactResolver.h>
#include <limits>
#include <iostream>

namespace nPhysics
{
	cParticleContactResolver::cParticleContactResolver(size_t iterations)
		: mIterations(iterations)
		, mIterationsUsed(0)
	{

	}

	void cParticleContactResolver::SetIterations(size_t iterations)
	{
		mIterations = iterations;
	}

	void cParticleContactResolver::ResolveContacts(cParticleContact* contactArray, size_t numContacts, float deltaTime)
	{
		size_t i;

		mIterationsUsed = 0;
		while (mIterationsUsed < mIterations)
		{
			// Find the contact with the largest closing velocity
			float max = std::numeric_limits<float>::max();
			unsigned int maxIndex = numContacts;
			for (i = 0; i < numContacts; i++)
			{
				float sepVel = contactArray[i].CalculateSeparatingVelocity();
				if (sepVel < max &&
					(sepVel < 0 || contactArray[i].mPenetration > 0))
				{
					max = sepVel;
					maxIndex = i;
				}
			}

			// Do we have anything worth resolving?
			if (maxIndex == numContacts) break;

			// Resolve this contact
			contactArray[maxIndex].Resolve(deltaTime);

			// Update the interpenetrations for all particles
			cParticleContact* contact = &contactArray[maxIndex];
			for (i = 0; i < numContacts; i++)
			{
				if (contactArray[i].mParticle0 == contact->mParticle0)
				{
					contactArray[i].mPenetration -= glm::dot(contact->mMovement0, contactArray[i].mContactNormal);
				}
				else if (contactArray[i].mParticle0 == contact->mParticle1)
				{
					contactArray[i].mPenetration -= glm::dot(contact->mMovement1, contactArray[i].mContactNormal);
				}
				if (contactArray[i].mParticle1)
				{
					if (contactArray[i].mParticle1 == contact->mParticle0)
					{
						contactArray[i].mPenetration += glm::dot(contact->mMovement0, contactArray[i].mContactNormal);
					}
					else if (contactArray[i].mParticle1 == contact->mParticle1)
					{
						contactArray[i].mPenetration += glm::dot(contact->mMovement1, contactArray[i].mContactNormal);
					}
				}
			}

			mIterationsUsed++;
		}
		//std::cout << "solved in " << mIterationsUsed << " iterations" << std::endl;

	}
}