#pragma once

// Exit maze State is for the player to locate the exit of the graph using AStar from anywhere in the graph
// Re-calculates path if the next node in the path is occupied

#include "sFSMState.h"

class sExitMazeState : public sFSMState
{
public:
	sExitMazeState();
	virtual ~sExitMazeState();

	virtual std::string Update(float dt, Node* currentNode, glm::vec3 lookDirection, int direction);
	virtual void OnEnterState(Node* currentNode);
	virtual void OnExitState();

protected:

private:
	std::vector<Node*> pathToTarget;

	float timeToNextAction;
	int dirToRotate;
};