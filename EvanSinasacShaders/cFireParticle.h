#pragma once

#include <physics/cParticle.h>
#include "cMesh.h"

namespace nPhysics
{
	class cFireParticle : public cParticle
	{
	public:

		cFireParticle(float mass, const glm::vec3& position, float timeUntilDeath);
		virtual ~cFireParticle();

		virtual void Integrate(float deltaTime);

		//void SetTimeToDie(float tToD);

		cMesh* m_Mesh;

	protected:

	private:
		float timeToDie;
		float timeAlive;
	};
}