#pragma once
#include <Windows.h>

class Window
{
public:
	Window();
	Window(const Window&) = default;
	Window& operator=(const Window&) = delete;
	
	virtual ~Window();
public:
	bool IsRun();

	RECT GetClientWindowRect();
	//void SetHWND(HWND hWnd);

	virtual void OnCreate() = 0;
	virtual void OnUpdate() = 0;
	virtual void OnDestroy();
	
protected:
	HWND hWnd;
	bool isRun;

	bool isInited;

private:
	void Init();
	void Release();
	bool Update();
};

