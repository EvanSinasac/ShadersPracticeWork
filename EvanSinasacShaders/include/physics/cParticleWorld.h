#pragma once
#include <physics/cParticle.h>
#include <physics/cParticleForceRegistry.h>
#include <physics/cParticleContact.h>
#include <physics/iParticleContactGenerator.h>
#include <physics/cParticleContactResolver.h>

//#include "../../cBullet.h";
//#include "../../cLaser.h";
//#include "../../cCannonShot.h";

//#include <physics/cProjectile.h>

#include <vector>

namespace nPhysics
{
	class cParticleWorld
	{
	private:
		std::vector<cParticle*> mParticles;
		cParticleForceRegistry* mForceRegistry;
		// The list of contacts.
		cParticleContact* mContacts;
		// The maximum number of contacts allowed.
		size_t mMaxContacts;
		// All of the contact generators.
		std::vector<iParticleContactGenerator*> mContactGenerators;
		// The particle contact resolver.
		cParticleContactResolver mResolver;
		// The world should calculate the number of iterations
		// to give the contact resolver each frame.
		bool mCalculateIterations;
	public:
		cParticleWorld(size_t maxContacts, size_t iterations = 0);
		virtual ~cParticleWorld();

		cParticleWorld() = delete;
		cParticleWorld(cParticleWorld& other) = delete;
		cParticleWorld& operator=(cParticleWorld& other) = delete;

		bool AddParticle(cParticle* particle);
		bool RemoveParticle(cParticle* particle);
		size_t NumParticles();
		void TimeStep(float deltaTime);
		size_t GenerateContacts();

		void IntegrateParticles(float deltaTime);

		cParticleForceRegistry* GetForceRegistry() const;
		
		bool AddContactGenerator(iParticleContactGenerator* generator);
		bool RemoveContactGenerator(iParticleContactGenerator* generator);
	};
}

