#include "pch.h"
#include "Input/Input.h"

namespace sr {
	bool Input::Initialize()
	{
		//keyboard
		int numkeys;
		const bool* keyState = SDL_GetKeyboardState(&numkeys);
		m_keyStates.resize(numkeys);

		std::copy(keyState, keyState + numkeys, m_keyStates.begin());
		m_prevKeyStates = m_keyStates;

		return false;
	}

	void Input::Shutdown()
	{
		//useless chud
	}

	void Input::Update()
	{
		//keyboard
		m_prevKeyStates = m_keyStates;
		const bool* keyState = SDL_GetKeyboardState(NULL);

		std::copy(keyState, keyState + m_keyStates.size(), m_keyStates.begin());

		//mouse
		m_prevButtonStates = m_buttonStates;
		m_buttonStates = SDL_GetMouseState(&m_mousePos.x, &m_mousePos.y);

	}
	uint32_t Input::GetMouseBit(MouseButton button) const
	{
		return SDL_BUTTON_MASK((uint32_t)button);
	}
}

