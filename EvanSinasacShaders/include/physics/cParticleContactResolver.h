#pragma once
#include <physics/cParticleContact.h>

namespace nPhysics
{
	/// <summary>
	/// The contact resolution routine for particle contacts.
	/// One resolver instance is generally shared for the entire simulation.
	/// </summary>
	class cParticleContactResolver
	{
	protected:
		// Holds the number of iterations allowed.
		size_t  mIterations;
		// This is a performance tracking value - we keep a record
		// of the actual number of iterations used.
		size_t mIterationsUsed;
	public:
		/// <summary>
		/// Creates a new contact resolver
		/// </summary>
		/// <param name="iterations">The number of iterations to use when solving contacts.</param>
		cParticleContactResolver(size_t iterations);
		/// <summary>
		/// Sets the number of iterations that can be used.
		/// </summary>
		/// <param name="iterations">The number of iterations to use when solving contacts.</param>
		void SetIterations(size_t iterations);

		/// <summary>
		/// Resolves a set of particle contacts for both penetration and velocity.
		/// 
		/// </summary>
		/// <param name="contactArray"></param>
		/// <param name="numContact"></param>
		/// <param name="deltaTime"></param>
		void ResolveContacts(cParticleContact* contactArray, size_t numContacts, float deltaTime);
	};
}