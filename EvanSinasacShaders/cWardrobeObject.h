#pragma once

#include "iEntity.h"
#include "Graph.h"

class cWardrobeObject : public iEntity
{
public:
	cWardrobeObject(glm::vec3 startPos, Node* startNode);
	virtual ~cWardrobeObject();

	virtual void Update(float deltaTime);
protected:

private:
	Node* m_Node;
};