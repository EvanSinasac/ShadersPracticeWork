#include "cTreasureObject.h"
#include <iostream>

cTreasureObject::cTreasureObject(glm::vec3 startPosition, Node* startNode)
{
	this->active = true;
	this->m_Node = startNode;
	this->type = iEntity::ENTITY_TYPE::TREASURE;

	this->m_Mesh = new cMesh();
	this->m_Mesh->meshName = "dfk_chest_02_closed_XYZ_N_RGBA_UV_transformed.ply";
	this->m_Mesh->positionXYZ = startPosition;
	this->m_Mesh->setUniformScale(1.0f);
	this->m_Mesh->bDontLight = false;
	this->m_Mesh->bUseStencil = true;
	this->m_Mesh->clearTextureRatiosToZero();
	this->m_Mesh->textureOperator = 4;
	this->m_Mesh->textureNames[1] = "dfk_small_containers_blue_basecolor.bmp";
	this->m_Mesh->textureNames[2] = "dfk_small_containers_normal.bmp";
	this->m_Mesh->textureNames[3] = "dfk_small_containers_SmMetAO.bmp";
	this->m_Mesh->textureRatios[1] = 1.0f;
	this->m_Mesh->textureRatios[2] = 1.0f;
	this->m_Mesh->textureRatios[3] = 0.3f;

	this->m_LowResMesh = new cMesh();
	this->m_LowResMesh->meshName = "Isosphere_Smooth_Normals.ply";
	this->m_LowResMesh->positionXYZ = startPosition;
	this->m_LowResMesh->setUniformScale(0.4f);
	this->m_LowResMesh->bDontLight = false;
	this->m_LowResMesh->bUseStencil = true;
	this->m_LowResMesh->clearTextureRatiosToZero();
	this->m_LowResMesh->textureOperator = 1;
	this->m_LowResMesh->textureNames[1] = "dfk_small_containers_blue_basecolor.bmp";
	this->m_Mesh->textureRatios[1] = 1.0f;
}
cTreasureObject::~cTreasureObject()
{

}

void cTreasureObject::Update(float deltaTime)
{
	if (active)
	{
		if (this->m_Node->isOccupied && this->m_Node->occupiedBy == 0)
		{
			this->m_Mesh->bIsVisible = false;
			this->m_LowResMesh->bIsVisible = false;
			active = false;
			std::cout << "Woo Hoo!  You found some treasure!" << std::endl;
		}
	}
}