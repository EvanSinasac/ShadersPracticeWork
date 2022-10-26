#include <physics/cParticleWorld.h>
#include <algorithm>
#include <iostream>

namespace nPhysics
{
	cParticleWorld::cParticleWorld(size_t maxContacts, size_t iterations)
		: mResolver(iterations)
	{
		mCalculateIterations = (iterations == 0);
		mForceRegistry = new cParticleForceRegistry();
		mContacts = new cParticleContact[maxContacts];
		mMaxContacts = maxContacts;
	}
	cParticleWorld::~cParticleWorld()
	{
		delete mForceRegistry;
		delete[] mContacts;
	}

	bool cParticleWorld::AddParticle(cParticle* particle)
	{
		// 1) Null check, because you can't add nothing
		if (!particle)
		{
			return false; // nothing was added
		}
		// 2) Check if we have it.
		//    If no: Add it, return true to indicate that we added it.
		//    If yes: Return false, to indicate that nothing was added.
		std::vector<cParticle*>::iterator itParticle = std::find(mParticles.begin(), mParticles.end(), particle);
		if (itParticle == mParticles.end())
		{
			// Not there! add it.
			mParticles.push_back(particle);
			return true;
		}
		return false; // nothing was added.
	}

	bool cParticleWorld::RemoveParticle(cParticle* particle)
	{
		// 1) Null check, because you can't remove nothing
		if (!particle)
		{
			return false; // nothing was removed
		}
		// 2) Remove any force generators associated with it.
		mForceRegistry->Deregister(particle);
		// 3) Check if we have it.
		//    If no: Add it, return false to indicate that nothign was removed
		//    If yes: Removed it. Return true to indicate that it was removed
		std::vector<cParticle*>::iterator itParticle = std::find(mParticles.begin(), mParticles.end(), particle);
		if (itParticle == mParticles.end())
		{
			return false; // nothing was removed.
		}
		mParticles.erase(itParticle);
		return true; // found it. removed it.
	}

	void cParticleWorld::TimeStep(float deltaTime)
	{
		// 1) Update Force Generators
		mForceRegistry->UpdateForces(deltaTime);

		// 2) Integrate the particles
		IntegrateParticles(deltaTime);

		// 3) Generate contacts
		size_t numContactsGenerated = GenerateContacts();

		// 4) Resolve contacts
		if (numContactsGenerated)
		{
			if (mCalculateIterations)
			{
				mResolver.SetIterations(numContactsGenerated * 2);
			}
			mResolver.ResolveContacts(mContacts, numContactsGenerated, deltaTime);
		}
	}

	size_t cParticleWorld::GenerateContacts()
	{
		size_t limit = mMaxContacts;
		cParticleContact* nextContact = mContacts;

		for (iParticleContactGenerator* g : mContactGenerators)
		{
			size_t used = g->AddContact(nextContact, limit, mParticles);
			limit -= used;
			nextContact += used;

			// We've run out of contacts to fill. This means we're missing collisions.
			if (limit <= 0)
			{
				std::cout << __FUNCSIG__ << " -> TOO MANY CONTACTS!" << std::endl;
				break;
			}
		}

		return mMaxContacts - limit;
	}

	size_t cParticleWorld::NumParticles()
	{
		return mParticles.size();
	}

	void cParticleWorld::IntegrateParticles(float deltaTime)
	{
		// loop over our particles and tell them to integrate
		std::vector<cParticle*>::iterator itParticle = mParticles.begin();
		while (itParticle != mParticles.end())
		{
			(*itParticle)->Integrate(deltaTime);
			itParticle++;
		}
	}
	cParticleForceRegistry* cParticleWorld::GetForceRegistry() const
	{
		return mForceRegistry;
	}

	bool cParticleWorld::AddContactGenerator(iParticleContactGenerator* generator)
	{
		// 1) Null check, because you can't add nothing.
		if (!generator)
		{
			return false;
		}
		// 2) Check if we have it
		//     if no:  Add it. Then return true to indicate that it was added.
		//     if yes: Return false to indicate that nothing was added.
		std::vector<iParticleContactGenerator*>::iterator itGenerator = std::find(mContactGenerators.begin(), mContactGenerators.end(), generator);
		if (itGenerator == mContactGenerators.end())
		{
			// Not there! Add it then return true;
			mContactGenerators.push_back(generator);
			return true;
		}
		return false; // nothing was added.
	}
	bool cParticleWorld::RemoveContactGenerator(iParticleContactGenerator* generator)
	{
		// 1) Null check, because you can't remove nothing.
		if (!generator)
		{
			return false;
		}
		// 2) Check if we have it
		//     if yes:  Remove it. Then return true to indicate that it was removed.
		//     if not: Return false to indicate that nothing was removed.
		std::vector<iParticleContactGenerator*>::iterator itGenerator = std::find(mContactGenerators.begin(), mContactGenerators.end(), generator);
		if (itGenerator == mContactGenerators.end())
		{
			return false; // nothing was removed.
		}
		mContactGenerators.erase(itGenerator);
		return true; // found it. removed it.
	}
}