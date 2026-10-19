#pragma once
#include "Serialization/Json.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/transform.hpp>

namespace sr
{
	struct Transform
	{
		glm::vec3 pos{ 0.0f };
		glm::vec3 rot{ 0.0f };
		glm::vec3 scale{ 1.0f };

		glm::mat4 matrix;

		const glm::mat4& GetMatrix() {
			auto mxt = glm::translate(pos);
			auto mxr = glm::eulerAngleXYZ(glm::radians(rot.y), glm::radians(rot.x), glm::radians(rot.z));
			auto mxs = glm::scale(glm::mat4{ 1.0f }, scale);

			matrix = mxt * mxr * mxs;

			return matrix;
		}

		void Read(const json::value_t& value) {
			JSON_READ_REQ(value, pos);
			JSON_READ_REQ(value, rot);
			JSON_READ_REQ(value, scale);

		}
	};
}