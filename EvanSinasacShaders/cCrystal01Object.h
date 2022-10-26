#pragma once

#include "iEntity.h"
#include "Graph.h"

class cCrystal01Object : public iEntity
{
public:
	cCrystal01Object(glm::vec3 startPos, Node* startNode);
	virtual ~cCrystal01Object();

	virtual void Update(float deltaTime);
protected:

private:
	Node* m_Node;
};