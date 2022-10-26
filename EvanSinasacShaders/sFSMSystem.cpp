#include "sFSMSystem.h"

sFSMSystem::sFSMSystem()
	: mIsRunning(false)
	, mCurrentState(0)
{

}

sFSMSystem::~sFSMSystem()
{

}

void sFSMSystem::Start()
{
	if (mStateVec.size() == 0)
	{
		printf("Finite State Machine is empty!  Cannot start!\n");
		return;
	}

	mIsRunning = true;
	TransitionToState(mStateVec[0], nullptr);
	return;
}

void sFSMSystem::AddState(sFSMState* state)
{
	sFSMState* key = state;
	if (std::find(mStateVec.begin(), mStateVec.end(), key) == mStateVec.end())
	{
		mStateVec.push_back(state);
	}
	return;
}

std::string sFSMSystem::Update(float dt, Node* currentNode, glm::vec3 lookDirection, int direction)
{
	if (!mIsRunning)
		return "notRunning";

	// Check to make sure we have a state
	if (mCurrentState == 0)
	{
		printf("Error: The current state is null");
		mIsRunning = false;
		return "noState";
	}

	//curPos = mCurrentState->Update(dt, curPos, this->mPubState);
	std::string message = mCurrentState->Update(dt, currentNode, lookDirection, direction);

	if (mCurrentState->IsDone())
	{
		sFSMState* newState = mCurrentState->GetTransition();
		TransitionToState(newState, currentNode);
	}

	return message;

}

void sFSMSystem::TransitionToState(sFSMState* state, Node* currentNode)
{
	if (state == 0)
	{
		printf("Error: FSMSystem::TransitionToState: The state is null\n");
		mIsRunning = false;
		return;
	}

	if (mCurrentState != 0)
	{
		mCurrentState->OnExitState();
	}

	mCurrentState = state;
	mCurrentState->OnEnterState(currentNode);

	return;
}

void sFSMSystem::Reset()
{
	if (mCurrentState != 0)
	{
		mCurrentState->OnExitState();
	}

	mCurrentState = 0;
	mIsRunning = false;

	return;
}