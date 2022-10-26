#pragma once

#include "iEntity.h"
#include "Graph.h"

class cBedObject : public iEntity
{
public:
	cBedObject(glm::vec3 startPos, Node* startNode);
	virtual ~cBedObject();

	virtual void Update(float deltaTime);
protected:

private:
	Node* m_Node;
};