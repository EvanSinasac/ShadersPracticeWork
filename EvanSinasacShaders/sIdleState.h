#pragma once
#include "sFSMState.h"

class sIdleState : public sFSMState
{
public:
	sIdleState();
	virtual ~sIdleState();

	virtual std::string Update(float dt, Node* currentNode, glm::vec3 lookDirection, int direction);
	virtual void OnEnterState(Node* currentNode);
	virtual void OnExitState();
};