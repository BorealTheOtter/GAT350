#pragma once
#include "Serialization/Json.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/norm.hpp>

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
			auto mxr = glm::eulerAngleXYZ(glm::radians(rot.x), glm::radians(rot.y), glm::radians(rot.z));
			auto mxs = glm::scale(glm::mat4{ 1.0f }, scale);

			matrix = mxt * mxr * mxs;

			return matrix;
		}

		glm::vec3 Forward() const {
			auto mxr = glm::eulerAngleXYZ(glm::radians(rot.x), glm::radians(rot.y), glm::radians(rot.z));
			return mxr * glm::vec4{ 0,0,1, 0 };
		}

		glm::vec3 Up() const {
			auto mxr = glm::eulerAngleXYZ(glm::radians(rot.x), glm::radians(rot.y), glm::radians(rot.z));
			return mxr * glm::vec4{ 0,1,0, 0 };
		}

		glm::vec3 Right() const {
			auto mxr = glm::eulerAngleXYZ(glm::radians(rot.x), glm::radians(rot.y), glm::radians(rot.z));
			return mxr * glm::vec4{ 1,0,0, 0 };
		}

		void Read(const json::value_t& value) {
			JSON_READ_REQ(value, pos);
			JSON_READ_REQ(value, rot);
			JSON_READ_REQ(value, scale);

		}
	};
}