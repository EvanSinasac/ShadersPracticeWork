#pragma once
//#include <extern_includes.h>
#include "../../GLMCommon.h"
#include "../../GLCommon.h"


namespace nPhysics
{
	// Generate a random number between 0 and 1
	float getRandom();

	// Generate a random number between zero and a given high value
	float getRandom(float high);

	// Generate a random number in a given range
	float getRandom(float low, float high);

	// Generate a glm::vec3 with a random direction and specified length
	glm::vec3 getRandomVec3(float length = 1.0f);
}