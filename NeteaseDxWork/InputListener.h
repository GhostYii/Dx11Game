#pragma once
#include "Point.h"

class InputListener
{
public:
	InputListener() = default;
	virtual ~InputListener() = default;

public:
	virtual void OnKeyDown(int keycode) = 0;
	virtual void OnKeyUp(int keycode) = 0;

	virtual void OnMouseMove(const Point& delta) = 0;
	virtual void OnMouseKeyDown(int mouseKey) = 0;
	virtual void OnMouseKeyUp(int mouseKey) = 0;
};