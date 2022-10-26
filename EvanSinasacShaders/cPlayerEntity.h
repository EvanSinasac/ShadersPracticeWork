#pragma once

#include "iEntity.h"
#include "cMesh.h"
#include "Graph.h"

#include "sFSMSystem.h"	

class cPlayerEntity : public iEntity
{
public:
	cPlayerEntity();
	cPlayerEntity(glm::vec3 startPos, glm::vec3 startLookAt, Node* startNode);
	virtual ~cPlayerEntity();

	virtual void Update(float dt);

	void Move(std::string directionToMove);
	void Rotate(std::string directionToRotate);

	void StartCheating();

	//cMesh* m_Mesh;

	glm::vec3 position;
	glm::vec3 lookAt;

	bool cheating;
protected:

private:
	
	//DIRECTIONS direction;
	int direction;

	Node* m_CurrNode;

	sFSMSystem* m_FSMSystem;
	sFSMState* m_IdleState;
	sFSMState* m_SearchState;

	unsigned int soundIndex;
	int nextSoundIndex;

	void PlayFootstepSound();

};