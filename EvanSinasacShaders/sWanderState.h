#pragma once

#include "sFSMState.h"

class sWanderState : public sFSMState
{
public:
	sWanderState();
	virtual ~sWanderState();

	virtual std::string Update(float dt, Node* currentNode, glm::vec3 lookDirection, int direction);
	virtual void OnEnterState(Node* currentNode);
	virtual void OnExitState();

protected:

private:
	Node* nextNode;
	//glm::vec3 

	float timeToNextAction;
	unsigned int stage;
	int dirToRotate;
};