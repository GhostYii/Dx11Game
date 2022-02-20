#pragma once
#include "InputListener.h"
#include "Point.h"
#include <unordered_set>

class InputSystem
{
public:
	InputSystem() = default;
	~InputSystem() = default;

public:
	static InputSystem* GetInstance();

	void AddListener(InputListener* listener);
	void RemoveListener(InputListener* listener);

	void Update();

	void SetCursorPosition(const Point& position);
	void SetCursorVisiable(bool visiable);

	Point GetMouseDelta();
private:
	std::unordered_set<InputListener*> listenerMap;

	unsigned char keyStates[256] = {};
	unsigned char prevKeyStates[256] = {};

	Point prevMousePos = {};

private:
	bool isFirstTimeMoveMouse = true;
};

