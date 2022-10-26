#pragma once
#include <physics/iParticleContactGenerator.h>

namespace nPhysics
{
	/// <summary>
	/// cParticleConstraint is the base class for any constraint between two particles
	/// </summary>
	class cParticleConstraint : public iParticleContactGenerator
	{
	protected:
		cParticle* mParticleA;
		cParticle* mParticleB;
	public:
		cParticleConstraint(cParticle* particleA, cParticle* particleB);

		cParticleConstraint() = delete;
		cParticleConstraint(const cParticleConstraint& other) = delete;
		cParticleConstraint& operator=(const cParticleConstraint& other) = delete;

		virtual ~cParticleConstraint();

		float CurrentSeparation() const;

		cParticle* GetParticleA();
		cParticle* GetParticleB();

		// AddContact will be overridden by inheriting classes
		//virtual size_t AddContact(cParticleContact* contact, size_t limit, std::vector<cParticle*>& particles) const = 0;
	};


	class cParticleRodConstraint : public cParticleConstraint
	{
	protected:
		float mLength;
	public:
		cParticleRodConstraint(cParticle* particleA, cParticle* particleB);

		cParticleRodConstraint() = delete;
		cParticleRodConstraint(const cParticleRodConstraint& other) = delete;
		cParticleRodConstraint& operator=(const cParticleRodConstraint& other) = delete;

		virtual size_t AddContact(cParticleContact* contact, size_t limit, std::vector<cParticle*>& particles) const;
	};

	class cParticleRopeConstraint : public cParticleConstraint
	{
	protected:
		float mLength;
	public: 
		cParticleRopeConstraint(cParticle* particleA, cParticle* particleB);

		cParticleRopeConstraint() = delete;
		cParticleRopeConstraint(const cParticleRopeConstraint& other) = delete;
		cParticleRopeConstraint& operator=(const cParticleRopeConstraint& other) = delete;

		virtual size_t AddContact(cParticleContact* contact, size_t limit, std::vector<cParticle*>& particles) const;
	};
}