#pragma once

#include <vector>
#include <string>

#include "FSMState.h"

#include "Graph.h"

#include "GLMCommon.h"

class FSMSystem
{
public:
	FSMSystem();
	virtual ~FSMSystem();

	void AddState(FSMState* state);
	void TransitionToState(FSMState* state, glm::vec3 curPos);

	glm::vec3 Update(float dt, glm::vec3 curPos);
	void Start();
	void Reset();

	int mPubState;

private:
	std::vector<FSMState*> mStateVec;	// keeps track of all the states inside our FSM
	bool mIsRunning;					// is our FSM active

	FSMState* mCurrentState;			// current active state
};