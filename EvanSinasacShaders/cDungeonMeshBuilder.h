#pragma once

#include <string>
#include "cMesh.h"

class cDungeonMeshBuilder
{
public:
	enum class TypeOfMesh
	{
		FLOOR = 0,
		WALL = 1,
		DOOR = 2,
		SECRETDOOR = 3,
		PORTCULLIS = 4,
		STAIRS = 5,
		LIGHT_FIXTURE = 6,
		//BOOKSHELF = 7,	// decided to make them in the object entities
		//CHAIR = 8,
		//CRYSTAL_1 = 9,
		//CRYSTAL_2 = 10,
		//TABLE = 11,
		//CHEST = 12,
	};

	cDungeonMeshBuilder();
	virtual ~cDungeonMeshBuilder();

	cMesh* MakeMesh(TypeOfMesh type, glm::vec3 scale);

protected:

private:

};