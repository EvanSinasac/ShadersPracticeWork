#include "sIdleState.h"

sIdleState::sIdleState()
	: sFSMState("Idle State")
{

}

sIdleState::~sIdleState()
{

}

std::string sIdleState::Update(float dt, Node* currentNode, glm::vec3 lookDirection, int direction)
{
	//stateID = 0;
	mCurrentCondition = 1;
	return "idling";
}

void sIdleState::OnEnterState(Node* currentNode)
{
	mCurrentCondition = 0;
	//printf("IdleState: Entered\n");
}

void sIdleState::OnExitState()
{
	//printf("IdleState: Exited\n");
	mCurrentCondition = 0;
}