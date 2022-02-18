#pragma once
#include <Windows.h>

class Window
{
public:
	Window();
	Window(const Window&) = default;
	Window& operator=(const Window&) = delete;
	
	virtual ~Window() = default;
public:
	bool IsRun();

	bool Init();
	bool Broadcast();
	bool Release();

	RECT GetClientWindowRect();
	void SetHWND(HWND hWnd);

	virtual void OnCreate() = 0;
	virtual void OnUpdate() = 0;
	virtual void OnDestroy();
	
protected:
	HWND hWnd;
	bool isRun;
};

