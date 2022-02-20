#include "InputSystem.h"
#include <Windows.h>

InputSystem* InputSystem::GetInstance()
{
	static InputSystem instance;
	return &instance;
}

void InputSystem::AddListener(InputListener* listener)
{
	listenerMap.insert(listener);
}

void InputSystem::RemoveListener(InputListener* listener)
{
	listenerMap.erase(listener);
}

void InputSystem::Update()
{
	POINT currentMousePosition = {};
	GetCursorPos(&currentMousePosition);

	if (isFirstTimeMoveMouse)
	{
		prevMousePos = Point(currentMousePosition.x, currentMousePosition.y);
		isFirstTimeMoveMouse = false;
	}

	if (currentMousePosition.x != prevMousePos.x || currentMousePosition.y != prevMousePos.y)
	{
		std::unordered_set<InputListener*>::iterator iter = listenerMap.begin();

		while (iter != listenerMap.end())
		{
			(*iter)->OnMouseMove(Point(currentMousePosition.x, currentMousePosition.y));
			++iter;
		}
	}

	prevMousePos = Point(currentMousePosition.x, currentMousePosition.y);

	if (GetKeyboardState(keyStates))
	{
		for (unsigned int i = 0; i < 256; i++)
		{
			// key down
			if (keyStates[i] & 0x80)
			{
				std::unordered_set<InputListener*>::iterator iter = listenerMap.begin();

				while (iter != listenerMap.end())
				{
					if (i == VK_LBUTTON)
					{
						if (keyStates[i] != prevKeyStates[i])
						{
							(*iter)->OnMouseKeyDown(0);
						}
					}
					else if (i == VK_RBUTTON)
					{
						if (keyStates[i] != prevKeyStates[i])
						{
							(*iter)->OnMouseKeyDown(1);
						}
					}
					else if (i == VK_MBUTTON)
					{
						if (keyStates[i] != prevKeyStates[i])
						{
							(*iter)->OnMouseKeyDown(2);
						}
					}
					else
						(*iter)->OnKeyDown(i);
					
					++iter;
				}
			}
			// key up
			else
			{
				if (keyStates[i] != prevKeyStates[i])
				{
					std::unordered_set<InputListener*>::iterator iter = listenerMap.begin();

					while (iter != listenerMap.end())
					{
						if (i == VK_LBUTTON)						
							(*iter)->OnMouseKeyUp(0);						
						else if (i == VK_RBUTTON)						
							(*iter)->OnMouseKeyUp(1);						
						else if (i == VK_MBUTTON)						
							(*iter)->OnMouseKeyUp(2);						
						else
							(*iter)->OnKeyUp(i);
						
						++iter;
					}
				}
			}
		}

		memcpy(prevKeyStates, keyStates, sizeof(unsigned char) * 256);
	}
}

void InputSystem::SetCursorPosition(const Point& position)
{
	SetCursorPos(position.x, position.y);
}

void InputSystem::SetCursorVisiable(bool visiable)
{
	ShowCursor(visiable);
}

Point InputSystem::GetMouseDelta()
{
	POINT curPos;
	GetCursorPos(&curPos);
	return Point(curPos.x - prevMousePos.x, curPos.y - prevMousePos.y);
}
