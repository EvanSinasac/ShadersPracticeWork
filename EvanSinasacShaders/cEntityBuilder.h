#pragma once

#include <string>
#include "iEntity.h"
#include "Graph.h"

class cEntityBuilder
{
public:
	enum class TypeOfEntity
	{
		PLAYER = 0,
		WANDER_ENEMY = 1,
		LISTEN_ENEMY = 2,
		BED = 3,
		BOOKSHELF = 4,
		CHAIR = 5,
		CRYSTAL_1 = 6,
		CRYSTAL_2 = 7,
		TABLE = 8,
		WARDROBE = 9,
		TREASURE = 10,
	};

	cEntityBuilder();
	virtual ~cEntityBuilder();

	iEntity* MakeEntity(TypeOfEntity type, glm::vec3 startPos, Node* node);
protected:

private:

};