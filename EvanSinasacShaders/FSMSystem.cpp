#include "FSMSystem.h"

FSMSystem::FSMSystem()
	: mIsRunning(false)
	, mCurrentState(0)
{

}

FSMSystem::~FSMSystem()
{

}

void FSMSystem::Start()
{
	if (mStateVec.size() == 0)
	{
		printf("Finite State Machine is empty!  Cannot start!\n");
		return;
	}

	mIsRunning = true;
	TransitionToState(mStateVec[0], glm::vec3(0.0f));
	return;
}

void FSMSystem::AddState(FSMState* state)
{
	FSMState* key = state;
	if (std::find(mStateVec.begin(), mStateVec.end(), key) == mStateVec.end())
	{
		mStateVec.push_back(state);
	}
	return;
}

glm::vec3 FSMSystem::Update(float dt, glm::vec3 curPos)
{
	if (!mIsRunning)
		return curPos;

	// Check to make sure we have a state
	if (mCurrentState == 0)
	{
		printf("Error: The current state is null");
		mIsRunning = false;
		return curPos;
	}

	curPos = mCurrentState->Update(dt, curPos, this->mPubState);
	
	if (mCurrentState->IsDone())
	{
		FSMState* newState = mCurrentState->GetTransition();
		TransitionToState(newState, curPos);
	}

	return curPos;

}

void FSMSystem::TransitionToState(FSMState* state, glm::vec3 curPos)
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
	mCurrentState->OnEnterState(curPos);

	return;
}

void FSMSystem::Reset()
{
	if (mCurrentState != 0)
	{
		mCurrentState->OnExitState();
	}

	mCurrentState = 0;
	mIsRunning = false;

	return;
}