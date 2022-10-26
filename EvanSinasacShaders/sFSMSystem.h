#pragma once

#include <vector>
#include <string>

#include "sFSMState.h"

#include "Graph.h"

#include "GLMCommon.h"

class sFSMSystem
{
public:
	sFSMSystem();
	virtual ~sFSMSystem();

	void AddState(sFSMState* state);
	void TransitionToState(sFSMState* state, Node* currentNode);

	std::string Update(float dt, Node* currentNode, glm::vec3 lookDirection, int direction);
	void Start();
	void Reset();

	//int mPubState;
protected:

private:
	std::vector<sFSMState*> mStateVec;	// keeps track of all the states inside our FSM
	bool mIsRunning;					// is our FSM active

	sFSMState* mCurrentState;			// current active state
};