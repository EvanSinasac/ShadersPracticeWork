#include "cFootprints.h"

cFootprints::cFootprints()
{
	this->m_Mesh = new cMesh();
	this->m_Mesh->meshName = "Quad_1_sided_aligned_on_XY_plane.ply";
	this->m_Mesh->positionXYZ = glm::vec3(0.0f);
	this->m_Mesh->setUniformScale(2.5f);
	this->m_Mesh->clearTextureRatiosToZero();
	this->m_Mesh->textureNames[0] = "Dinosaur-Footprints-clipart-2.bmp";
	this->m_Mesh->textureRatios[0] = 1.0f;
	this->m_Mesh->bHasDiscardTexture = true;
	this->m_Mesh->discardTexture = "Dinosaur-Footprints-clipart-2.bmp";

	this->m_Mesh->bUseStencil = false;

	this->m_LowResMesh = this->m_Mesh;

	this->type = iEntity::ENTITY_TYPE::FOOTPRINTS;

	active = false;

}
cFootprints::cFootprints(glm::vec3 startPos)
{
	this->m_Mesh = new cMesh();
	this->m_Mesh->meshName = "Quad_1_sided_aligned_on_XY_plane.ply";
	this->m_Mesh->positionXYZ = glm::vec3(startPos.x, 0.1f, startPos.z);
	this->m_Mesh->setUniformScale(2.5f);
	this->m_Mesh->clearTextureRatiosToZero();
	//this->m_Mesh->textureNames[0] = "Dinosaur-Footprints-clipart-2.bmp";
	//this->m_Mesh->textureRatios[0] = 1.0f;
	this->m_Mesh->bHasDiscardTexture = true;
	//this->m_Mesh->discardTexture = "Dinosaur-Footprints-clipart-2.bmp";
	this->m_Mesh->bIsVisible = false;
	this->m_Mesh->bUseStencil = false;

	this->m_LowResMesh = this->m_Mesh;

	this->type = iEntity::ENTITY_TYPE::FOOTPRINTS;

	active = false;
	timer = 0.0f;
	
}
cFootprints::~cFootprints()
{

}

void cFootprints::Update(float dt)
{
	if (active)
	{
		timer += dt;
		this->m_Mesh->alphaTransparency = (10.0f - timer) / 10.0f;
		if (timer >= 10.0f)
		{
			active = false;
			this->m_Mesh->bIsVisible = false;
		}
	}
}

void cFootprints::Activate()
{
	this->m_Mesh->bIsVisible = true;
	this->m_Mesh->alphaTransparency = 1.0f;
	active = true;
	timer = 0.0f;
}

void cFootprints::SetTextures(int typeOfTexture)
{
	switch (typeOfTexture)
	{
	case 0:
		this->m_Mesh->textureNames[0] = "Dinosaur-Footprints-clipart-2.bmp";
		this->m_Mesh->textureRatios[0] = 1.0f;
		this->m_Mesh->bHasDiscardTexture = true;
		this->m_Mesh->discardTexture = "Dinosaur-Footprints-clipart-2.bmp";
		this->m_Mesh->textureOperator = 1;
		break;
	case 1:
		this->m_Mesh->textureNames[0] = "treads.bmp";	//
		this->m_Mesh->textureRatios[0] = 1.0f;
		this->m_Mesh->bHasDiscardTexture = true;
		this->m_Mesh->discardTexture = "treads.bmp";
		this->m_Mesh->textureOperator = 2;
		break;
	default:
		break;
	}
}
void cFootprints::SetOrientation(glm::vec3 newOri)
{
	this->m_Mesh->orientationXYZ = newOri;
}