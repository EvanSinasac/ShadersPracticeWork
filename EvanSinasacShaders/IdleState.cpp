#include "IdleState.h"

IdleState::IdleState()
	: FSMState("Idle State")
{

}

IdleState::~IdleState()
{

}

glm::vec3 IdleState::Update(float dt, glm::vec3 curPos, int& stateID)
{
	stateID = 0;
	mCurrentCondition = 1;
	return curPos;
}

void IdleState::OnEnterState(glm::vec3 curPos)
{
	mCurrentCondition = 0;
	printf("IdleState: Entered\n");
}

void IdleState::OnExitState()
{
	printf("IdleState: Exited\n");
}