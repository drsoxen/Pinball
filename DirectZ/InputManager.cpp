#include "Common.h"

InputManager* InputManager::m_InputManager = 0;

InputManager* InputManager::GetInstance()
{
	if(m_InputManager == 0)
	{
		m_InputManager = new InputManager();
	}
	return m_InputManager;
}
void InputManager::Destroy()
{
	if (m_InputManager) 
	delete m_InputManager;
	m_InputManager = 0;
}
void InputManager::set_MousePress(int MouseButton)
{
	MouseButtons[MouseButton] = true;
}
void InputManager::set_MouseRelease(int MouseButton)
{
	MouseButtons[MouseButton] = false;
}
void InputManager::set_KeyPress(WPARAM wParam)
{
	Keys[wParam] = true;
	DevConsole::GetInstance()->logFormattedMessage("key Press: %c\n", (char)wParam);
}
void InputManager::set_KeyRelease(WPARAM wParam)
{
	Keys[wParam] = false;
	DevConsole::GetInstance()->logFormattedMessage("key released: %c\n", (char)wParam);
}
void InputManager::set_MouseMove(LPARAM lParam)
{
	MousePos.X = LOWORD(lParam);
	MousePos.Y = HIWORD(lParam);
}
bool InputManager::get_Key(char key)
{
	if(Keys[key] == true)
	{
		return true;
	}
	else
	{
		return false;
	}
}
bool InputManager::get_Mouse(int MouseButton)
{
	if(MouseButtons[MouseButton] == true)
	{
		return true;
	}
	else
	{
		return false;
	}
}
	
COORD InputManager::get_MouseMove()
{
	return MousePos;
}

InputManager::InputManager()
{
	memset(&Keys, 0 , sizeof(Keys)/sizeof(bool));
	memset(&MouseButtons, 0 , sizeof(MouseButtons)/sizeof(bool));
}

InputManager::~InputManager()
{

}

