#pragma once

#include "iEntity.h"
#include "Graph.h"

class cBookshelfObject : public iEntity
{
public:
	cBookshelfObject(glm::vec3 startPos, Node* startNode);
	virtual ~cBookshelfObject();

	virtual void Update(float deltaTime);
protected:

private:
	Node* m_Node;
};