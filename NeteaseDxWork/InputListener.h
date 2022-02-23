#pragma once
#include "Point.h"

class InputSystem;

class InputListener
{
	friend class InputSystem;
public:
	InputListener() = default;
	virtual ~InputListener() = default;

protected:
	virtual void OnKeyDown(int keycode) = 0;
	virtual void OnKey(int keycode) = 0;
	virtual void OnKeyUp(int keycode) = 0;

	virtual void OnMouseMove(const Point& mousePosition) = 0;
	virtual void OnMouseKeyDown(int mouseKey) = 0;
	virtual void OnMouseKey(int mouseKey) = 0;
	virtual void OnMouseKeyUp(int mouseKey) = 0;
};