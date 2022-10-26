#pragma once

#include "iEntity.h"
#include "Graph.h"

class cChairObject : public iEntity
{
public:
	cChairObject(glm::vec3 startPos, Node* startNode);
	virtual ~cChairObject();

	virtual void Update(float deltaTime);
protected:

private:
	Node* m_Node;
};