#pragma once
#include "GLMCommon.h"
#include "cMesh.h"

static const glm::vec3 vec3NORTH = glm::vec3(0.0f, 0.0f, 2.5f);
static const glm::vec3 vec3NORTH_EAST = glm::vec3(-2.5f, 0.0f, 2.5f);
static const glm::vec3 vec3EAST = glm::vec3(-2.5f, 0.0f, 0.0f);
static const glm::vec3 vec3SOUTH_EAST = glm::vec3(-2.5f, 0.0f, -2.5f);
static const glm::vec3 vec3SOUTH = glm::vec3(0.0f, 0.0f, -2.5f);
static const glm::vec3 vec3SOUTH_WEST = glm::vec3(2.5f, 0.0f, -2.5f);
static const glm::vec3 vec3WEST = glm::vec3(2.5f, 0.0f, 0.0f);
static const glm::vec3 vec3NORTH_WEST = glm::vec3(2.5f, 0.0f, 2.5f);

class iEntity
{
public:
	enum class DIRECTIONS : int
	{
		NORTH = 0,
		NORTH_EAST = 1,
		EAST = 2,
		SOUTH_EAST = 3,
		SOUTH = 4,
		SOUTH_WEST = 5,
		WEST = 6,
		NORTH_WEST = 7,
	};

	enum class ENTITY_TYPE
	{
		PLAYER,
		ENEMY_LISTEN,
		ENEMY_WANDER,
		TREASURE,
		OBJECT,
		TORCH,
		FOOTPRINTS,
	};

	virtual ~iEntity() {};

	virtual void Update(float dt) = 0;

	ENTITY_TYPE type;
	cMesh* m_Mesh;

	cMesh* m_LowResMesh;
	//cMesh* m_HiResMesh;
};