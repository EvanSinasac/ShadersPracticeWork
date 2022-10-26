#pragma once
#include "../vec2.hpp"
#include "../vec3.hpp"
#include "../vec4.hpp"
#include <limits>
namespace glm
{
	// set the values of a glm::vec2
	inline void set(glm::vec2& v, float x, float y)
	{
		v.x = x; v.y = y;
	}
	// set the values of a glm::vec4
	inline void set(glm::vec3& v, float x, float y, float z)
	{
		v.x = x; v.y = y; v.z = z;
	}
	// set the values of a glm::vec4
	inline void set(glm::vec4& v, float x, float y, float z, float w)
	{
		v.x = x; v.y = y; v.z = z; v.w = w;
	}
	// set the values of a glm::vec4, leaving w alone
	inline void set(glm::vec4& v, float x, float y, float z)
	{
		v.x = x; v.y = y; v.z = z;
	}
	// set the values to whatever is smallest
	inline void setMin(glm::vec3& v, float x, float y, float z)
	{
		if (x < v.x) v.x = x;
		if (y < v.y) v.y = y;
		if (z < v.z) v.z = z;
	}
	// set the values to whatever is smallest
	inline void setMin(glm::vec3& v, const glm::vec3& other)
	{
		setMin(v, other.x, other.y, other.z);
	}
	// set the values to std::numeric_limits<float>::min()
	inline void setMin(glm::vec3& v)
	{
		v.x = std::numeric_limits<float>::min();
		v.y = std::numeric_limits<float>::min();
		v.z = std::numeric_limits<float>::min();
	}
	// set the values to whatever is largest
	inline void setMax(glm::vec3& v, float x, float y, float z)
	{
		if (x > v.x) v.x = x;
		if (y > v.y) v.y = y;
		if (z > v.z) v.z = z;
	}
	// set the values to whatever is largest
	inline void setMax(glm::vec3& v, const glm::vec3& other)
	{
		setMax(v, other.x, other.y, other.z);
	}
	// set the values to std::numeric_limits<float>::max()
	inline void setMax(glm::vec3& v)
	{
		v.x = std::numeric_limits<float>::max();
		v.y = std::numeric_limits<float>::max();
		v.z = std::numeric_limits<float>::max();
	}
	// check if it's between
	inline bool isContained(float value, float minBounds, float maxBounds)
	{
		if (value < minBounds) return false;
		if (value > maxBounds) return false;
		return true;
	}
	// check if it's between
	inline bool isContained(const glm::vec2& check, const glm::vec2& minBounds, const glm::vec2& maxBounds)
	{
		return isContained(check.x, minBounds.x, maxBounds.x)
			&& isContained(check.y, minBounds.y, maxBounds.y);
	}
	// check if it's between
	inline bool isContained(const glm::vec3& check, const glm::vec3& minBounds, const glm::vec3& maxBounds)
	{
		return isContained(check.x, minBounds.x, maxBounds.x)
			&& isContained(check.y, minBounds.y, maxBounds.y)
			&& isContained(check.z, minBounds.z, maxBounds.z);
	}
}