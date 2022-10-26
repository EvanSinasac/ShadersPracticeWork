#include "sFSMState.h"

sFSMState::sFSMState()
	: mCurrentCondition(0)
{

}
sFSMState::sFSMState(std::string name)
	: mCurrentCondition(0)
	, mStateName(name)
{

}
sFSMState::~sFSMState()
{

}

// Action to take when current state is active - what our state is doing while active
std::string sFSMState::Update(float dt, Node* currentNode, glm::vec3 lookDirection, int direction)
{
	return "nothing";
}
// Actions to take when entering a new state
void sFSMState::OnEnterState(Node* currentNode)
{
	printf("Entering %s state... \n", mStateName.c_str());
}
// Actions to take when exiting our current state
void sFSMState::OnExitState()
{
	printf("Exiting %s state... \n", mStateName.c_str());
}
// a condition has been met that triggers a transition
bool sFSMState::IsDone()
{
	return mCurrentCondition != 0;
}
// add a new transition to our map, includes condition as a key
void sFSMState::AddTransition(int condition, sFSMState* state)
{
	std::map<int, sFSMState*>::iterator it = mTransitionMap.find(condition);
	if (it != mTransitionMap.end())
	{
		printf("FSMState::AddTransition: State already has this condition!\n");
		return;
	}

	mTransitionMap[condition] = state;

	return;
}
// returns state that we want to transition to
sFSMState* sFSMState::GetTransition()
{
	if (!IsDone())	return 0;
	return mTransitionMap[mCurrentCondition];
}