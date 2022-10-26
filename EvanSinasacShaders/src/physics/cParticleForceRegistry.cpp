#include <physics/cParticleForceRegistry.h>



namespace nPhysics
{
	cParticleForceRegistry::cParticleForceRegistry()
	{

	}
	cParticleForceRegistry::~cParticleForceRegistry()
	{

	}

	void cParticleForceRegistry::Register(cParticle* particle, iParticleForceGenerator* generator)
	{
		mRegistry.insert(std::make_pair(particle, generator));
	}

	void cParticleForceRegistry::Deregister(cParticle* particle, iParticleForceGenerator* generator)
	{
		registryRange range = mRegistry.equal_range(particle);
		for (registryIterator it = range.first; it != range.second; )
		{
			registryIterator eraseIt = it++;
			if (eraseIt->second == generator)
			{
				mRegistry.erase(eraseIt);
			}
		}
	}

	void cParticleForceRegistry::Deregister(cParticle* particle)
	{
		mRegistry.erase(particle);
	}

	void cParticleForceRegistry::Deregister(iParticleForceGenerator* generator)
	{
		for (registryIterator it = mRegistry.begin(); it != mRegistry.end(); )
		{
			registryIterator eraseIt = it++;
			if (eraseIt->second == generator)
			{
				mRegistry.erase(eraseIt);
			}
		}
	}

	void cParticleForceRegistry::UpdateForces(float deltaTime)
	{
		for (registryIterator it = mRegistry.begin(); it != mRegistry.end(); it++)
		{
			// generator->Updated(particle, deltaTime);
			it->second->Update(it->first, deltaTime);
		}
	}
}