#pragma once

#include <string>
#include <map>

#include "GLMCommon.h"

class FSMState
{
public:
	FSMState(std::string name);
	virtual ~FSMState();

	// Action to take when current state is active - what our state is doing while active
	virtual glm::vec3 Update(float dt, glm::vec3 curPos, int& stateID) = 0;
	// Actions to take when entering a new state
	virtual void OnEnterState(glm::vec3 curPos) = 0;
	// Actions to take when exiting our current state
	virtual void OnExitState() = 0;

	bool IsDone();	// a condition has been met that triggers a transition

	void AddTransition(int condition, FSMState* state);		// add a new transition to our map, includes condition as a key

	FSMState* GetTransition();	// returns state that we want to transition to

protected:
	FSMState();
	int mCurrentCondition;		// 0 = state has not performed any action that will cause a transition	1 = state has performed an action and is now ready to switch

private:
	std::string mStateName;
	std::map<int, FSMState*> mTransitionMap;
};