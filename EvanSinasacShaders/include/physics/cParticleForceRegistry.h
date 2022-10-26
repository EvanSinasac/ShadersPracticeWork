#pragma once
#include <physics/cParticle.h>
#include <physics/iParticleForceGenerator.h>
#include <vector>
#include <map>

namespace nPhysics
{
	class cParticleForceRegistry
	{
		typedef std::multimap<cParticle*, iParticleForceGenerator*> registryType;
		typedef std::multimap<cParticle*, iParticleForceGenerator*>::iterator registryIterator;
		typedef std::pair<cParticle*, iParticleForceGenerator*> registryPair;
		typedef std::pair<registryIterator, registryIterator> registryRange;
		
		registryType mRegistry;
	public:
		cParticleForceRegistry();
		~cParticleForceRegistry();

		cParticleForceRegistry(cParticleForceRegistry& other) = delete;
		cParticleForceRegistry& operator=(cParticleForceRegistry& other) = delete;

		void Register(cParticle* particle, iParticleForceGenerator* generator);
		void Deregister(cParticle* particle, iParticleForceGenerator* generator);
		void Deregister(cParticle* particle);
		void Deregister(iParticleForceGenerator* generator);

		void UpdateForces(float deltaTime);


	};
}
