#pragma once
#ifndef __INPUT_H__

#include "utils.h"



enum InputState //Put later in utils
{
	INPUT_FIRST_RELEASE,
	INPUT_RELEASE,
	INPUT_FIRST_PRESS,
	INPUT_HOLD
};

enum InputMouse //Put later in utils
{
	MOUSE_LEFT,
	MOUSE_MIDDLE,
	MOUSE_RIGHT,
};

class CInput
{
public:
	~CInput();

	InputState GetKeyState(char key)
	{
		return (KeyState[key]);
	}

	//Keyboard states
	/*********************************************/
	void KeyboardUp(unsigned char key, int x, int y)
	{
		cLastKeyPressed = key;

		KeyState[key] = INPUT_RELEASE;
		m_CurrentState = INPUT_RELEASE;
	}
	void KeyboardDown(unsigned char key, int x, int y)
	{
		cLastKeyPressed = key;

		KeyState[key] = INPUT_HOLD;
		m_CurrentState = INPUT_HOLD;
	}

	//Mouse states
	/*********************************************/
	void MouseClick(int button, int glutState, int x, int y)
	{
		if (button < 3)
		{
			m_MousePos = glm::vec3(x, y, 0.0f);
			MouseState[button] = (glutState == GLUT_DOWN) ? INPUT_HOLD : INPUT_RELEASE;
		}
	}

	void MouseMove(int x, int y) { m_MousePos = glm::vec3(x, y, 0.0f); };

	glm::vec3 GetMousePos() { return(m_MousePos); }

	InputState GetMouseState(int button)
	{
		if (MouseState[button] == INPUT_HOLD && button < 3)
		{
			return(INPUT_HOLD);
		}
		else
		{
			return (INPUT_RELEASE);
		}
	}


	void UpdateMousePicking();
	glm::vec3 GetLookDirection() { return(m_LookVec); }

	//Singleton setup
	/*********************************************/

	static CInput& GetInstance()
	{
		if (s_pInput == nullptr)
		{
			s_pInput = new CInput();
		}

		return(*s_pInput);
	}

	static void DestroyInstance()
	{
		delete s_pInput;
		s_pInput = nullptr;
	}

protected:
	static CInput* s_pInput;

private:
	CInput();
	CInput(const CInput& _kr) = delete;
	CInput& operator= (const CInput& _kr) = delete;
	/*********************************************/

private:
	InputState KeyState[255];
	InputState MouseState[3];

	glm::vec3 m_MousePos;
	glm::vec3 m_LookVec;

	char cLastKeyPressed;
	InputState m_CurrentState;
};

#endif