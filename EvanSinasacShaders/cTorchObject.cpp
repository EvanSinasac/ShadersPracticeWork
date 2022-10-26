#include "cTorchObject.h"
#include "globalThings.h"
#include "cFireParticle.h"
#include <physics/random_helpers.h>
#include <iostream>

cTorchObject::cTorchObject(cMesh* pMesh, unsigned int index)
	: lightIndex(index)
{
	baseLightAtten = 0.4f;
	baseLightAtten = 0.08f;
	baseMeshAtten = 0.75f;
	timer = 0.0f;

	type = ENTITY_TYPE::TORCH;
	this->m_Mesh = pMesh;

	//this->m_HiResMesh = pMesh;

	this->m_LowResMesh = new cMesh();
	this->m_LowResMesh->meshName = "Isosphere_Smooth_Normals.ply";
	this->m_LowResMesh->positionXYZ = pMesh->positionXYZ;
	this->m_LowResMesh->bDontLight = true;
	this->m_LowResMesh->bIsWireframe = false;
	this->m_LowResMesh->bUseWholeObjectDiffuseColour = true;
	this->m_LowResMesh->wholeObjectDiffuseRGBA = glm::vec4(0.3f, 0.0f, 0.0f, 1.0f);
	this->m_LowResMesh->setUniformScale(0.1f);
	this->m_LowResMesh->clearTextureRatiosToZero();
}

cTorchObject::~cTorchObject()
{

}

void cTorchObject::Update(float deltaTime)
{
	timer += deltaTime;
	//std::cout << "timer: " << timer << std::endl;
	if (timer > 0.05f)
	{
		//float randomAddedAttenLight = ((rand() % 100) / 100.0f + 1.0f) * gGetRandBetween<float>(1.0f, 2.0f);
		float randomAddedAttenLight = ((rand() % 10) + 1.0f) / 100.0f * gGetRandBetween<float>(1.0f, 2.0f);
		float randomAddedAttenMesh = randomAddedAttenLight / 5.0f;
		if (lightIndex < g_pTheLights->NUMBER_OF_LIGHTS)
		{
			::g_pTheLights->theLights[lightIndex].atten.y = baseLightAtten + randomAddedAttenLight;
		}
		this->m_Mesh->vec_pChildMeshes[0]->vec_pChildMeshes[0]->scale.z = baseMeshAtten + randomAddedAttenMesh * 3.0f;
		//this->m_Mesh->scale.z = baseMeshAtten + randomAddedAttenMesh;
		timer = 0.0f;

		if (glm::distance(this->m_Mesh->positionXYZ, ::cameraEye) < 30.0f)
		{	// only launch particles if the camera is near enough
			LaunchParticle();
		}
	}
	
	return;
}

void cTorchObject::LaunchParticle()
{
	glm::vec3 position = this->m_Mesh->positionXYZ +
		this->m_Mesh->vec_pChildMeshes[0]->vec_pChildMeshes[0]->positionXYZ;

	if (this->m_Mesh->orientationXYZ.y == glm::radians(90.0f))
	{
		position += glm::vec3(-0.35f, 0.0f, -0.35f);
	}
	else if (this->m_Mesh->orientationXYZ.y == glm::radians(180.0f))
	{
		position += glm::vec3(-0.7f, 0.0f, 0.0f);
	}
	else if (this->m_Mesh->orientationXYZ.y == glm::radians(270.0f))
	{
		position += glm::vec3(-0.35f, 0.0f, 0.35f);
	}

	nPhysics::cFireParticle* newFire = new nPhysics::cFireParticle(1.0f, 
		position, 3.0f);
	glm::vec3 initialVelocity(nPhysics::getRandom(-0.05f, 0.05f), 0.3f, nPhysics::getRandom(-0.05f, 0.05f));
	newFire->SetVelocity(initialVelocity);
	newFire->SetAcceleration(glm::vec3(0.0f, 0.0f, 0.0f));
	newFire->SetDamping(0.9f);
	newFire->SetRadius(0.05f);
	newFire->SetUseTimer(true);
	newFire->SetTimer(3.0f);
	::g_pWorld->AddParticle(newFire);
	::g_vec_pParticles.push_back(newFire);

	return;
}
