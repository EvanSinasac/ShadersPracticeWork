#include "cFSMEntity.h"

cFSMEntity::cFSMEntity(Graph* cpyGraph, glm::vec3 pos)
{
	this->m_FSMSystem = new FSMSystem();

	this->m_StateIdle = new IdleState();
	this->m_StateSearch = new SearchState();
	this->m_StateGather = new GatherState();
	this->m_StateReturn = new ReturnState();

	// setup transitions
	this->m_StateIdle->AddTransition(1, this->m_StateSearch);
	this->m_StateSearch->AddTransition(1, this->m_StateGather);
	this->m_StateSearch->AddTransition(2, 0);
	this->m_StateGather->AddTransition(1, this->m_StateReturn);
	this->m_StateReturn->AddTransition(1, this->m_StateSearch);

	this->m_FSMSystem->AddState(this->m_StateIdle);
	this->m_FSMSystem->AddState(this->m_StateSearch);
	this->m_FSMSystem->AddState(this->m_StateGather);
	this->m_FSMSystem->AddState(this->m_StateReturn);

	this->m_FSMSystem->Start();

	// Mesh stuff
	this->m_Mesh = new cMesh();
	this->m_Mesh->meshName = "Isosphere_Smooth_Normals.ply";
	this->m_Mesh->positionXYZ = pos;
	this->m_Mesh->orientationXYZ = glm::vec3(0.0f);
	this->m_Mesh->setUniformScale(0.3f);
	this->m_Mesh->bUseObjectDebugColour = true;
	this->m_Mesh->objectDebugColourRGBA = glm::vec4(0.8f, 0.6f, 0.4f, 1.0f);
	this->m_Mesh->clearTextureRatiosToZero();

	this->idleColour = glm::vec4(0.8f, 0.6f, 0.4f, 1.0f);
	this->searchColour = glm::vec4(0.8f, 0.6f, 0.4f, 1.0f);
	this->gatherColour = glm::vec4(0.8f, 0.4f, 0.8f, 1.0f);
	this->returnColour = glm::vec4(0.4f, 0.8f, 0.8f, 1.0f);

	this->m_GraphCopy = cpyGraph;
}
cFSMEntity::~cFSMEntity()
{

}

void cFSMEntity::Update(float dt)
{
	glm::vec3 newPos = this->m_FSMSystem->Update(dt, this->m_Mesh->positionXYZ);
	this->m_Mesh->positionXYZ = newPos;

	switch (this->m_FSMSystem->mPubState)
	{
	case 0:		// Idle
		this->m_Mesh->objectDebugColourRGBA = idleColour;
		break;
	case 1:		// Search
		this->m_Mesh->objectDebugColourRGBA = searchColour;
		break;
	case 2:		// Gather
		this->m_Mesh->objectDebugColourRGBA = gatherColour;
		break;
	case 3:		// Return
		this->m_Mesh->objectDebugColourRGBA = returnColour;
		break;
	default:
		this->m_Mesh->objectDebugColourRGBA = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);	// uh oh, something broke
	}
}