#pragma once
#include "GLMCommon.h"

struct Vertex
{
	Vertex() { x = y = z = 0; }
	Vertex(float iX, float iY, float iZ)
		: x(iX)
		, y(iY)
		, z(iZ)
	{

	}

	Vertex(glm::vec3 vec)
		: x(vec.x)
		, y(vec.y)
		, z(vec.z)
	{

	}

	float x, y, z;
};