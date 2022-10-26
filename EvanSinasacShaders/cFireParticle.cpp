#include "cFireParticle.h"

namespace nPhysics
{
	cFireParticle::cFireParticle(float mass, const glm::vec3& position, float timeUntilDeath)
		: cParticle(mass, position)
	{
		this->timeAlive = 0.0f;
		this->timeToDie = timeUntilDeath;

		this->m_Mesh = new cMesh();
		this->m_Mesh->meshName = "Isosphere_Smooth_Normals.ply";
		this->m_Mesh->positionXYZ = position;
		this->m_Mesh->setUniformScale(0.01f);
		this->m_Mesh->clearTextureRatiosToZero();
		this->m_Mesh->bUseWholeObjectDiffuseColour = true;
		this->m_Mesh->wholeObjectDiffuseRGBA = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
	}
	cFireParticle::~cFireParticle()
	{

	}

	void cFireParticle::Integrate(float deltaTime)
	{
		if (GetIsAlive())
		{
			if (GetInverseMass() == 0.f)
			{
				return; // static things don't move!
			}

			SetPosition(glm::vec3(GetPosition() + GetVelocity() * deltaTime));
			// F*(1/m) = a
			SetVelocity(glm::vec3(GetVelocity() + (GetAcceleration() / GetInverseMass() * deltaTime)));

			// apply damping
			SetVelocity(glm::vec3(GetVelocity() * glm::pow(GetDamping(), deltaTime)));

			this->timeAlive += deltaTime;

			if (timeAlive >= timeToDie)
			{
				SetIsAlive(false);
			}
			this->m_Mesh->positionXYZ = this->GetPosition();
			this->m_Mesh->wholeObjectDiffuseRGBA = glm::vec4(1.0f, timeAlive / timeToDie, 0.0f, 1.0f);
		}
		
	}
}