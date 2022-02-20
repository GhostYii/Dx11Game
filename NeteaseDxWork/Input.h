#pragma once
#include <Windows.h>

class Input
{
private:
	static const unsigned int NUMBER_OF_KEYS = 256U;
	static bool prevKeyboardState[NUMBER_OF_KEYS];

public:
	Input()
	{
		for (unsigned int keyNum = 0U; keyNum < NUMBER_OF_KEYS; ++keyNum)
		{
			prevKeyboardState[keyNum] = GetKeyDown(keyNum);
		}
	}

	~Input() = default;


	bool GetKeyDown(int key)
	{
		return (GetAsyncKeyState(key) & (1 << sizeof(SHORT) * 8 - 1));
	}

	bool GetKey(int key)
	{
		bool previousState = prevKeyboardState[key];

		prevKeyboardState[key] = GetKeyDown(key);

		return (prevKeyboardState[key] && !previousState);
	}

	bool GetKeyUp(int key)
	{
		bool previousState = prevKeyboardState[key];

		prevKeyboardState[key] = GetKeyDown(key);

		return (!prevKeyboardState[key] && previousState);
	}
};
