#include <physics/particle_contact_generators.h>
#include <iostream>
#include "../../globalThings.h"

namespace nPhysics
{
	cPlaneParticleContactGenerator::cPlaneParticleContactGenerator(glm::vec3 normal, float distance)
		: mNormal(glm::normalize(normal)),			// Safety check to ensure the normal is normalized
		distanceFromOrigin(distance)
	{

	}

	cPlaneParticleContactGenerator::~cPlaneParticleContactGenerator()
	{

	}

	size_t cPlaneParticleContactGenerator::AddContact(cParticleContact* contact, size_t limit, std::vector<cParticle*>& particles) const
	{
		size_t numContactsCreated = 0;
		for (std::vector<cParticle*>::iterator p = particles.begin(); p != particles.end(); p++)
		{
			float distanceToPlane = glm::dot((*p)->GetPosition(), mNormal) - distanceFromOrigin;
			
			// Fully behind plane
			bool behindPlane = distanceToPlane < (-1.0f * (*p)->GetRadius());	//-1.0f;
			if (behindPlane)
			{
				break;
				//return 0;
			}

			//Intersecting Negative Half-Space
			bool negativeHalfSpace = distanceToPlane <= (*p)->GetRadius();	//1.0f;
			if (negativeHalfSpace)
			{
				// Needs something here cause otherwise it's kinda jerky when it passes through the plane from behind, not sure what
				// Just need to make it good enough for rock and roll for Project 3 tho
				// NOPE! break;
			}
			

			// Intersects
			bool intersect = abs(distanceToPlane) <= (*p)->GetRadius();	//1.0f;
			if (intersect)
			{
				// create a contact
				contact->mContactNormal = mNormal;
				contact->mParticle0 = *p;
				contact->mParticle1 = nullptr;
				// I had this backwards (distanceToPlane-1.0f; -> 1.0f - distanceToPlane) //(*p)->GetRadius();
				contact->mPenetration = (*p)->GetRadius() - distanceToPlane;		
				contact->mRestitution =  0.9f;	// 0.7f;

				contact++;
				numContactsCreated++;
			}


			if (numContactsCreated >= limit)
			{
				return numContactsCreated;
			}
		}
		return numContactsCreated;
	}


	// Project 3, 2 particles contact
	cParticle2ParticleContactGenerator::cParticle2ParticleContactGenerator()
	{

	}
	cParticle2ParticleContactGenerator:: ~cParticle2ParticleContactGenerator()
	{

	}


	size_t cParticle2ParticleContactGenerator::AddContact(cParticleContact* contact, size_t limit, std::vector<cParticle*>& particles) const
	{
		size_t numContactsCreated = 0;
		// example loop we discussed in class, actual check and implementation added by me
		if (particles.size() > 0)
		{
			for (unsigned int indexA = 0; indexA < particles.size() - 1; indexA++)
			{
				float radiusA = particles[indexA]->GetRadius();
				for (unsigned int indexB = indexA + 1; indexB < particles.size(); indexB++)
				{
					float radiusB = particles[indexB]->GetRadius();
					// comparison between particles will never be repeated, and will never be the same particle
					//float distanceBetweenParticles = glm::distance(particles[indexA]->GetPosition(), particles[indexB]->GetPosition());
					// average of the two radi to check if the particles are intersecting
					//bool intersect = abs(distanceBetweenParticles) <= abs((particles[indexA]->GetRadius() + particles[indexB]->GetRadius()) / 2.0f);
					glm::vec3 separation = particles[indexA]->GetPosition() - particles[indexB]->GetPosition();

					float dist = glm::length(separation);

					if (radiusA + radiusB > dist)
					{

						// create a contact
						contact->mParticle0 = particles[indexA];
						contact->mParticle1 = particles[indexB];
						contact->mContactNormal = glm::normalize(separation);	//particles[indexB]->GetPosition() - particles[indexA]->GetPosition()	this was reversed...
						//	distanceToPlane = glm::dot((*p)->GetPosition(), mNormal) - distanceFromOrigin;
						contact->mPenetration = radiusA + radiusB - dist;//(particles[indexA]->GetRadius() + particles[indexB]->GetRadius()) - distanceBetweenParticles;

						contact->mRestitution = 0.1f;	// 0.7f;
						
						contact++;
						numContactsCreated++;
					}

					if (numContactsCreated >= limit)
					{
						return numContactsCreated;
					}
				} //end of for indexB
			} //end of for indexA
		} //end of if
		return numContactsCreated;
	} //end of AddContact for Particles2Particles
}