#pragma once

#include "iEntity.h"
#include "cMesh.h"
#include "Graph.h"

#include "sFSMSystem.h"		


class cWanderEnemy : public iEntity
{
public:
	cWanderEnemy();
	cWanderEnemy(glm::vec3 startPos, glm::vec3 startLookAt, Node* startNode);
	virtual ~cWanderEnemy();

	virtual void Update(float dt);

	void Move(std::string directionToMove);
	void Rotate(std::string directionToRotate);

	glm::vec3 position;
	glm::vec3 lookAt;

protected:

private:
	int direction;

	Node* m_CurrNode;


	sFSMSystem* m_FSMSystem;
	sFSMState* m_IdleState;
	sFSMState* m_WanderState;
};