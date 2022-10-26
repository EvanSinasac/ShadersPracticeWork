#pragma once

#include "sFSMState.h"

class sAttackState : public sFSMState
{
public:
	sAttackState();
	virtual ~sAttackState();

	virtual std::string Update(float dt, Node* currentNode, glm::vec3 lookDirection, int direction);
	virtual void OnEnterState(Node* currentNode);
	virtual void OnExitState();

protected:

private:
	Node* playerNode;
	float timeToNextAction;
	int dirToRotate;
};