#pragma once
#include "FSMState.h"
#include "MapInfo.h"

class IdleState : public FSMState
{
public:
	IdleState();
	virtual ~IdleState();

	virtual glm::vec3 Update(float dt, glm::vec3 curPos, int& stateID);
	virtual void OnEnterState(glm::vec3 curPos);
	virtual void OnExitState();
};