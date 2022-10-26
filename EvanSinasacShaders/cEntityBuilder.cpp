#include "cEntityBuilder.h"
#include "cWanderEnemy.h"
#include "cListeningEnemy.h"

#include "cBedObject.h"
#include "cBookshelfObject.h"
#include "cChairObject.h"
#include "cCrystal01Object.h"
#include "cCrystal02Object.h"
#include "cTableObject.h"
#include "cWardrobeObject.h"

#include "cTreasureObject.h"

cEntityBuilder::cEntityBuilder()
{

}
cEntityBuilder::~cEntityBuilder()
{

}

iEntity* cEntityBuilder::MakeEntity(TypeOfEntity type, glm::vec3 startPos, Node* node)
{
	iEntity* newEntity = nullptr;

	//PLAYER = 0,
	//WANDER_ENEMY = 1,
	//LISTEN_ENEMY = 2,
	//BED = 3,
	//BOOKSHELF = 4,
	//CHAIR = 5,
	//CRYSTAL_1 = 6,
	//CRYSTAL_2 = 7,
	//TABLE = 8,
	//TREASURE = 9,

	if (type == TypeOfEntity::PLAYER)
	{
		// haha, not using this
	}
	else if (type == TypeOfEntity::WANDER_ENEMY)
	{
		newEntity = new cWanderEnemy(startPos, vec3NORTH, node);
	}
	else if (type == TypeOfEntity::LISTEN_ENEMY)
	{
		newEntity = new cListeningEnemy(startPos, vec3NORTH, node);
	}
	else if (type == TypeOfEntity::BED)
	{
		newEntity = new cBedObject(startPos, node);
	}
	else if (type == TypeOfEntity::BOOKSHELF)
	{
		newEntity = new cBookshelfObject(startPos, node);
	}
	else if (type == TypeOfEntity::CHAIR)
	{
		newEntity = new cChairObject(startPos, node);
	}
	else if (type == TypeOfEntity::CRYSTAL_1)
	{
		newEntity = new cCrystal01Object(startPos, node);
	}
	else if (type == TypeOfEntity::CRYSTAL_2)
	{
		newEntity = new cCrystal02Object(startPos, node);
	}
	else if (type == TypeOfEntity::TABLE)
	{
		newEntity = new cTableObject(startPos, node);
	}
	else if (type == TypeOfEntity::WARDROBE)
	{
		newEntity = new cWardrobeObject(startPos, node);
	}
	else if (type == TypeOfEntity::TREASURE)
	{
		newEntity = new cTreasureObject(startPos, node);
	}
	
	return newEntity;
}