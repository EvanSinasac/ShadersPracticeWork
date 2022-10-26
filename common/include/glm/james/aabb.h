#pragma once
#include "inline_helpers.h"
#include "../vec3.hpp"
namespace glm
{
	// TODO: change this to be center-point and half-widths
	class aabb
	{
	public:
		glm::vec3 maxBounds;
		glm::vec3 minBounds;
		aabb()
		{
			setMin(maxBounds);
			setMax(minBounds);
		}
		aabb(const aabb& other, const glm::vec3& offset)
			: maxBounds(other.maxBounds + offset), minBounds(other.minBounds + offset)
		{ }

		inline void inflate(const glm::vec3& pt)
		{
			setMax(maxBounds, pt);
			setMin(minBounds, pt);
		}
		inline void getCenter(glm::vec3& centerOut)
		{
			centerOut = (maxBounds + minBounds) * 0.5f;
		}
		void getSize(glm::vec3& size) { size = maxBounds - minBounds; }
		float getXSize() { return maxBounds.x - minBounds.x; }
		float getYSize() { return maxBounds.y - minBounds.y; }
		float getZSize() { return maxBounds.z - minBounds.z; }
	};

	inline bool isContained(const aabb& box, const vec3& pt)
	{
		return isContained(pt, box.minBounds, box.maxBounds);
	}
	inline bool intersection(const aabb& a, const aabb& b)
	{
		if (a.maxBounds.x < b.minBounds.x) return false;
		if (a.maxBounds.y < b.minBounds.y) return false;
		if (a.maxBounds.z < b.minBounds.z) return false;
		if (b.maxBounds.x < a.minBounds.x) return false;
		if (b.maxBounds.y < a.minBounds.y) return false;
		if (b.maxBounds.z < a.minBounds.z) return false;
		return true;
	}
}