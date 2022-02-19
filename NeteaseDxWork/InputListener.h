#pragma once
#include "Point.h"

class InputLisenter
{
public:
	InputLisenter() = default;
	virtual ~InputLisenter() = default;

public:
	virtual void OnKeyDown(int keycode) = 0;
	virtual void OnKeyUp(int keycode) = 0;

	virtual void OnMouseMove(const Point& mousePosition) = 0;
	//virtual void OnMouseKeyDown(int mouseKey) = 0;
	//virtual void OnMouseKeyUp(int mouseKey) = 0;
};