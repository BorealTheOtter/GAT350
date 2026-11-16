#pragma once
#include "Serialization/Json.h"
#include "Editor/Editor.h"
#include <string>

#define CLASS_PROTOTYPE(classname) \
virtual std::unique_ptr<Object> Clone() const { return std::make_unique<classname>(*this); } \
virtual const char* GetClassName() const { return #classname; }

namespace sr {

	class Object : public IGUI
	{
	public:
		Object() = default;
		virtual ~Object() = default;

		CLASS_PROTOTYPE(Object);

		const std::string& GetName() { return m_name; }
		bool IsActive() const { return m_active; }
		void SetActive(bool active = true) { m_active = active; }


		void UpdateGui() override
		{
			ImGui::Text("%s", m_name.c_str());
		}

		virtual void Read(const json::value_t& value)
		{
			JSON_READ_NAME(value, "name", m_name);
			JSON_READ_NAME(value, "active", m_active);
		}
	protected:
		std::string m_name;
		bool m_active = true;
	};
}