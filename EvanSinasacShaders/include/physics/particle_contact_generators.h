// holds the classes I've made for different kinds of contact generators
// For Project 3, plane and particle
// and particle 2 particle
#pragma once
#include <physics/iParticleContactGenerator.h>

namespace nPhysics
{
	class cPlaneParticleContactGenerator : public iParticleContactGenerator
	{
	private:
		glm::vec3 mNormal;
		float distanceFromOrigin;
		
	public:
		cPlaneParticleContactGenerator(glm::vec3 normal, float distance);
		
		cPlaneParticleContactGenerator(const cPlaneParticleContactGenerator& other) = delete;
		cPlaneParticleContactGenerator& operator=(const cPlaneParticleContactGenerator& other) = delete;
		virtual ~cPlaneParticleContactGenerator();

		virtual size_t AddContact(cParticleContact* contact, size_t limit, std::vector<cParticle*>& particles) const;
	};

	class cParticle2ParticleContactGenerator : public iParticleContactGenerator
	{
	protected:

	public:
		cParticle2ParticleContactGenerator();
		virtual ~cParticle2ParticleContactGenerator();

		cParticle2ParticleContactGenerator(const cParticle2ParticleContactGenerator& other) = delete;
		cParticle2ParticleContactGenerator& operator=(const cParticle2ParticleContactGenerator& other) = delete;

		virtual size_t AddContact(cParticleContact* contact, size_t limit, std::vector<cParticle*>& particles) const;
	};
}