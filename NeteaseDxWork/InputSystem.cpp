#include "InputSystem.h"
#include <Windows.h>
using ButtonState = DirectX::Mouse::ButtonStateTracker::ButtonState;

InputSystem* InputSystem::instance = nullptr;

InputSystem::InputSystem()
{
	pKeyboard = std::make_unique<DirectX::Keyboard>();
	pMouse = std::make_unique<DirectX::Mouse>();
}

InputSystem* InputSystem::GetInstance()
{
	return instance;
}

void InputSystem::CreateInstance()
{
	if (InputSystem::instance)
		delete instance;
	instance = new InputSystem();

}

void InputSystem::ReleaseInstance()
{
	if (!InputSystem::instance)
		return;

	delete InputSystem::instance;
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
	auto keyboardState = pKeyboard->GetState();
	keyboardTracker.Update(keyboardState);
	auto mouseState = pMouse->GetState();
	mouseTracker.Update(mouseState);

	for (unsigned int i = 0; i < 256u; i++)
	{
		auto key = (DirectX::Keyboard::Keys)i;
		if (pKeyboard->GetState().IsKeyDown(key))
		{
			std::unordered_set<InputListener*>::iterator iter = listenerMap.begin();

			while (iter != listenerMap.end())
			{
				if (keyboardTracker.pressed.IsKeyDown(key))
					(*iter)->OnKeyDown(i);
				(*iter)->OnKey(i);
				++iter;
			}
		}

		if (keyboardTracker.IsKeyReleased(key))
		{
			std::unordered_set<InputListener*>::iterator iter = listenerMap.begin();

			while (iter != listenerMap.end())
			{
				(*iter)->OnKeyUp(i);
				++iter;
			}
		}
	}

	switch (mouseTracker.leftButton)
	{
	case ButtonState::HELD:
	{
		std::unordered_set<InputListener*>::iterator iter = listenerMap.begin();

		while (iter != listenerMap.end())
		{
			(*iter)->OnMouseKey(0);
			++iter;
		}
		break;
	}
	case ButtonState::RELEASED:
	{
		std::unordered_set<InputListener*>::iterator iter = listenerMap.begin();

		while (iter != listenerMap.end())
		{
			(*iter)->OnMouseKeyUp(0);
			++iter;
		}
		break;
	}
	case ButtonState::PRESSED:
	{
		std::unordered_set<InputListener*>::iterator iter = listenerMap.begin();

		while (iter != listenerMap.end())
		{
			(*iter)->OnMouseKeyDown(0);
			++iter;
		}
		break;
	}
	default:
		break;
	}
	switch (mouseTracker.rightButton)
	{
	case ButtonState::HELD:
	{
		std::unordered_set<InputListener*>::iterator iter = listenerMap.begin();

		while (iter != listenerMap.end())
		{
			(*iter)->OnMouseKey(1);
			++iter;
		}
		break;
	}
	case ButtonState::RELEASED:
	{
		std::unordered_set<InputListener*>::iterator iter = listenerMap.begin();

		while (iter != listenerMap.end())
		{
			(*iter)->OnMouseKeyUp(1);
			++iter;
		}
		break;
	}
	case ButtonState::PRESSED:
	{
		std::unordered_set<InputListener*>::iterator iter = listenerMap.begin();

		while (iter != listenerMap.end())
		{
			(*iter)->OnMouseKeyDown(1);
			++iter;
		}
		break;
	}
	default:
		break;
	}
	switch (mouseTracker.middleButton)
	{
	case ButtonState::HELD:
	{
		std::unordered_set<InputListener*>::iterator iter = listenerMap.begin();

		while (iter != listenerMap.end())
		{
			(*iter)->OnMouseKey(2);
			++iter;
		}
		break;
	}
	case ButtonState::RELEASED:
	{
		std::unordered_set<InputListener*>::iterator iter = listenerMap.begin();

		while (iter != listenerMap.end())
		{
			(*iter)->OnMouseKeyUp(2);
			++iter;
		}
		break;
	}
	case ButtonState::PRESSED:
	{
		std::unordered_set<InputListener*>::iterator iter = listenerMap.begin();

		while (iter != listenerMap.end())
		{
			(*iter)->OnMouseKeyDown(2);
			++iter;
		}
		break;
	}
	default:
		break;
	}

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
}

void InputSystem::SetCursorPosition(const Point& position)
{
	SetCursorPos(position.x, position.y);
}

void InputSystem::SetCursorVisiable(bool visiable)
{
	pMouse->SetVisible(visiable);
}

Point InputSystem::GetMouseDelta()
{
	POINT curPos;
	GetCursorPos(&curPos);
	return Point(curPos.x - prevMousePos.x, curPos.y - prevMousePos.y);
}
