#pragma once

#include "iEntity.h"
#include "Graph.h"

class cTableObject : public iEntity
{
public:
	cTableObject(glm::vec3 startPos, Node* startNode);
	virtual ~cTableObject();

	virtual void Update(float deltaTime);
protected:

private:
	Node* m_Node;
};