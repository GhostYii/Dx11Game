#pragma once
#include "InputListener.h"
#include <map>

class InputSystem
{
public:
	InputSystem() = default;
	~InputSystem() = default;

public:
	static InputSystem* GetInstance();

	void AddListener(InputLisenter* listener);
	void RemoveListener(InputLisenter* listener);

	void Update();
private:
	std::map<InputLisenter*, InputLisenter*> listenerMap;

private:	
	unsigned char keyStates[256] = {};
	unsigned char prevKeyStates[256] = {};
};

