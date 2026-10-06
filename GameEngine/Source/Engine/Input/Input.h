#pragma once
#include "Math/Vector2.h"

#include <vector>

namespace sr 
{
	class Input 
	{
	public:
		enum MouseButton
		{
			LEFT = 1,
			MIDDLE,
			RIGHT,
			MB4,
			MB5
		};


	public:
		bool Initialize();
		void Shutdown();

		void Update();

		//keyboard
		bool GetKeyDown(int key) const { return m_keyStates[key]; }
		bool GetPrevKeyDown(int key) const { return m_prevKeyStates[key]; }
		bool GetKeyPressed(int key) const { return m_keyStates[key] && !m_prevKeyStates[key]; }
		bool GetKeyReleased(int key) const { return !m_keyStates[key] && m_prevKeyStates[key]; }

		//mouse
		sr::Vector2 GetMousePos() const { return m_mousePos; }
		bool GetMouseDown(MouseButton button) const { return m_buttonStates & GetMouseBit(button); }
		bool GetPrevMouseDown(MouseButton button) const { return m_prevButtonStates & GetMouseBit(button); }
		bool GetMousePressed(MouseButton button) const { return GetMouseDown(button) && !GetPrevMouseDown(button); }
		bool GetMouseReleased(MouseButton button) const { return !GetMouseDown(button) && GetPrevMouseDown(button); }

	private:
		uint32_t GetMouseBit(MouseButton button) const;

	private:
		//keyboard
		std::vector<bool> m_keyStates;
		std::vector<bool> m_prevKeyStates;
		
		//mouse
		uint32_t m_buttonStates = 0;
		uint32_t m_prevButtonStates = 0;

		sr::Vector2 m_mousePos{ 0,0 };

	
	};

}