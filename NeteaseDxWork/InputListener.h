#pragma once
class InputLisenter
{
public:
	InputLisenter() = default;
	virtual ~InputLisenter() = default;

public:
	virtual void OnKeyDown(int keycode) = 0;
	virtual void OnKeyUp(int keycode) = 0;
};