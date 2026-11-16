#pragma once
#include "Serialization/Json.h"
#include "Editor/Editor.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/norm.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace sr
{
	struct Transform : public IGUI
	{
		glm::vec3 pos{ 0.0f };
		glm::quat rot{glm::identity<glm::quat>()};
		glm::vec3 scale{ 1.0f };

		void SetRotation(const glm::vec3& ea) {
			rot = glm::quat(glm::radians(ea));
		}

		glm::vec3 GetRotation() const { return glm::degrees(glm::eulerAngles(rot)); }

		void SetRotation(const glm::quat& qr) {
			rot = glm::normalize(qr);
		}

		const glm::mat4& GetMatrix() const {
			auto mxt = glm::translate(pos);
			auto mxr = glm::toMat4(rot);
			auto mxs = glm::scale(glm::mat4{ 1.0f }, scale);

			glm::mat4 matrix = mxt * mxr * mxs;

			return matrix;
		}

		glm::vec3 Forward() const {return rot * glm::vec3{ 0,0,-1 };}

		glm::vec3 Up() const {return rot * glm::vec3{ 0,1,0};}

		glm::vec3 Right() const {return rot * glm::vec3{ 1,0,0};}

		void UpdateGui() override {

			ImGui::DragFloat3("Position", glm::value_ptr(pos), 0.1f);
			glm::vec3 euler = GetRotation();

			if (ImGui::DragFloat3("Rotation", glm::value_ptr(euler), 0.1f))
			{
				SetRotation(euler);
			}
			ImGui::DragFloat3("Scale", glm::value_ptr(scale), 0.1f);
		}

		void Read(const json::value_t& value) {
			JSON_READ_REQ(value, pos);

			glm::vec3 euler;
			JSON_READ_NAME_REQ(value, "rot", euler);
			JSON_READ_REQ(value, scale);

		}
	};
}