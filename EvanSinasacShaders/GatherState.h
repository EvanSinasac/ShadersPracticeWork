#pragma once

#include "FSMState.h"

#include "MapInfo.h"

class GatherState : public FSMState {
public:
	GatherState(void);
	virtual ~GatherState(void);

	virtual glm::vec3 Update(float dt, glm::vec3 curPos, int& stateID);

	virtual void OnEnterState(glm::vec3 curPos);
	virtual void OnExitState(void);

private:
	float m_Timer;
};
