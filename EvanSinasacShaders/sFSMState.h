#pragma once

#include <string>
#include <map>
#include "Graph.h"

#include "GLMCommon.h"

class sFSMState
{
public:
	sFSMState(std::string name);
	virtual ~sFSMState();

	// Action to take when current state is active - what our state is doing while active
	virtual std::string Update(float dt, Node* currentNode, glm::vec3 lookDirection, int direction) = 0;
	// Actions to take when entering a new state
	virtual void OnEnterState(Node* currentNode) = 0;
	// Actions to take when exiting our current state
	virtual void OnExitState() = 0;

	bool IsDone();	// a condition has been met that triggers a transition

	void AddTransition(int condition, sFSMState* state);		// add a new transition to our map, includes condition as a key

	sFSMState* GetTransition();	// returns state that we want to transition to

protected:
	sFSMState();
	int mCurrentCondition;		// 0 = state has not performed any action that will cause a transition	1 = state has performed an action and is now ready to switch

private:
	std::string mStateName;
	std::map<int, sFSMState*> mTransitionMap;
};