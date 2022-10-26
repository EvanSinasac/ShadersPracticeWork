#pragma once

#include "sFSMState.h"

class sApproachState : public sFSMState
{
public:
	sApproachState();
	virtual ~sApproachState();

	virtual std::string Update(float dt, Node* currentNode, glm::vec3 lookDirection, int direction);
	virtual void OnEnterState(Node* currentNode);
	virtual void OnExitState();

protected:

private:
	std::vector<Node*> pathToTarget;

	float timeToNextAction;
	int dirToRotate;
};