#pragma once
#include <physics/cParticle.h>

namespace nPhysics
{
	/// <summary>
	/// Contains all the information about a collision that is required to resolve it.
	/// </summary>
	class cParticleContact
	{
	public:
		// The first (or only) particle involved in the contact.
		cParticle* mParticle0;
		// The second (may be null) particle involved in the contact.
		cParticle* mParticle1;
		// The restitution coefficient at the contact.
		float mRestitution;
		// The direction of the contact in world coordinates.
		glm::vec3 mContactNormal;
		// The depth of penetration at the contact.
		float mPenetration;

		// The amount that mParticle0 is moved by during interpenetration resolution;
		// Do not set this when creating the contact.
		glm::vec3 mMovement0;
		// The amount that mParticle1 is moved by during interpenetration resolution.
		// Do not set this when creating the contact.
		glm::vec3 mMovement1;

		// Resolve this contact, for both velocity and interpenetration.
		void Resolve(float deltaTime);
		// Calculates the separating velocity at this contact.
		float CalculateSeparatingVelocity() const;
		// Handles impulse calculations for this collision.
		void ResolveVelocity(float deltaTime);
		// Handles interpenetration resolution for this collision.
		void ResolvePosition(float deltaTime);
	};
}