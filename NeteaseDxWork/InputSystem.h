#pragma once
#include "InputListener.h"
#include "Point.h"
#include <unordered_set>
#include <vector>

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

	bool GetKeyDown(int keycode);
	bool GetKey(int keycode);
	bool GetKeyUp(int keycode);

private:
	std::unordered_set<InputListener*> listenerMap;
	unsigned char keyStates[256u] = {};	
	unsigned char prevKeyStates[256u] = {};

	bool prevKeyboardStates[256u] = {};

	Point prevMousePos = {};

private:
	bool isFirstTimeMoveMouse = true;	
	static InputSystem* instance;
};

