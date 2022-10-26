#pragma once
#include "../vec3.hpp"
#include "../mat4x4.hpp"
#include "../geometric.hpp"
#include "../ext/matrix_transform.hpp"

namespace glm
{
	inline mat4 rotation_from_to(const glm::vec3& from, const glm::vec3& to)
	{
		glm::vec3 v = cross(to, from);
		float angle = acos(dot(to, from) / (length(to) * length(from)));
		return rotate(glm::mat4(1.f), angle, normalize(v));
	}
}