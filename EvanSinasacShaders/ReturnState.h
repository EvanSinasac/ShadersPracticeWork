#pragma once

#include "FSMState.h"
#include "MapInfo.h"

#include <vector>
#include "Graph.h"

class ReturnState : public FSMState
{
public:
	ReturnState();
	virtual ~ReturnState();

	virtual glm::vec3 Update(float dt, glm::vec3 curPos, int& stateID);
	virtual void OnEnterState(glm::vec3 curPos);
	virtual void OnExitState();

private:
	//Node* targetNode;

	std::vector<Node*> pathToTarget;
	float m_Timer;
};