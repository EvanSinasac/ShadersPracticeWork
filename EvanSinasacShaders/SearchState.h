#pragma once

#include "FSMState.h"
#include "MapInfo.h"
#include "Graph.h"

#include <vector>

class SearchState : public FSMState {
public:
	SearchState(void);
	virtual ~SearchState(void);

	virtual glm::vec3 Update(float dt, glm::vec3 curPos, int& stateID);

	virtual void OnEnterState(glm::vec3 curPos);
	virtual void OnExitState(void);

private:
	//Node* targetNode;
	
	std::vector<Node*> pathToTarget;
};
