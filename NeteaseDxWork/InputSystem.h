#pragma once
#include "InputListener.h"
#include "Point.h"
#include <unordered_set>
#include <vector>
#include <Keyboard.h>
#include <Mouse.h>

class InputSystem
{
private:
	InputSystem();
	~InputSystem() = default;

public:
	static InputSystem* GetInstance();
	static void CreateInstance();
	static void ReleaseInstance();

	void AddListener(InputListener* listener);
	void RemoveListener(InputListener* listener);

	void Update();

	void SetCursorPosition(const Point& position);
	void SetCursorVisiable(bool visiable);

	Point GetMouseDelta();

private:
	std::unordered_set<InputListener*> listenerMap;
	Point prevMousePos = {};
	std::unique_ptr<DirectX::Keyboard> pKeyboard;
	std::unique_ptr<DirectX::Mouse> pMouse;

	DirectX::Keyboard::KeyboardStateTracker keyboardTracker;
	DirectX::Mouse::ButtonStateTracker mouseTracker;
	
private:
	bool isFirstTimeMoveMouse = true;	
	static InputSystem* instance;
};

