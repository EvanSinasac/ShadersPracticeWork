#pragma once

#include "sFSMState.h"

class sEnclosedWanderState : public sFSMState
{
public:
	sEnclosedWanderState();
	virtual ~sEnclosedWanderState();

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