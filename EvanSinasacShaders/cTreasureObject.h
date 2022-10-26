#pragma once

#include "iEntity.h"
#include "Graph.h"

class cTreasureObject : public iEntity
{
public:
	cTreasureObject(glm::vec3 startPosition, Node* startNode);
	virtual ~cTreasureObject();

	virtual void Update(float deltaTime);
protected:

private:
	Node* m_Node;
	bool active;
};