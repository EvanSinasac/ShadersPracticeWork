#pragma once

#include "iEntity.h"
#include "Graph.h"

class cCrystal02Object : public iEntity
{
public:
	cCrystal02Object(glm::vec3 startPos, Node* startNode);
	virtual ~cCrystal02Object();

	virtual void Update(float deltaTime);
protected:

private:
	Node* m_Node;
};