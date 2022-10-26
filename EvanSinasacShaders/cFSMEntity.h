#pragma once

#include "FSMSystem.h"
#include "MapInfo.h"
#include "IdleState.h"
#include "SearchState.h"
#include "GatherState.h"
#include "ReturnState.h"

#include "cMesh.h"

#include "Graph.h"

class cFSMEntity
{
public:
	cFSMEntity(Graph* cpyGraph, glm::vec3 pos);
	~cFSMEntity();

	void Update(float dt);

	cMesh* m_Mesh;
protected:

private:
	FSMState* m_StateIdle;
	FSMState* m_StateSearch;
	FSMState* m_StateGather;
	FSMState* m_StateReturn;

	FSMSystem* m_FSMSystem;

	Graph* m_GraphCopy;

	glm::vec4 idleColour;
	glm::vec4 searchColour;
	glm::vec4 gatherColour;
	glm::vec4 returnColour;
};