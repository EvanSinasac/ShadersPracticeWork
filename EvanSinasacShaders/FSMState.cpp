#include "FSMState.h"

// Constructor & Destructor
FSMState::FSMState() 
	: mCurrentCondition(0) { }

FSMState::FSMState(std::string name)
	: mCurrentCondition(0)
	, mStateName(name) { }

FSMState::~FSMState()
{

}

// Transitions

void FSMState::AddTransition(int condition, FSMState* state)
{
	std::map<int, FSMState*>::iterator it = mTransitionMap.find(condition);
	if (it != mTransitionMap.end())
	{
		printf("FSMState::AddTransition: State already has this condition!\n");
		return;
	}

	mTransitionMap[condition] = state;

	return;
}

FSMState* FSMState::GetTransition()
{
	if (!IsDone())	return 0;
	return mTransitionMap[mCurrentCondition];
}

bool FSMState::IsDone()
{
	return mCurrentCondition != 0;
}


// Actions
glm::vec3 FSMState::Update(float dt, glm::vec3 curPos, int& stateID)
{
	stateID = 0;
	return curPos;
}

void FSMState::OnEnterState(glm::vec3 curPos)
{
	printf("Entering %s state... \n", mStateName.c_str());
}

void FSMState::OnExitState()
{
	printf("Exiting %s state... \n", mStateName.c_str());
}