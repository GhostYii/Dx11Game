#pragma once
#include <Windows.h>

class Window
{
	friend LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
public:
	Window();
	Window(const Window&) = default;
	Window& operator=(const Window&) = delete;
	
	virtual ~Window();
public:
	bool IsRun();

	RECT GetClientWindowRect();

protected:
	virtual void OnCreate() = 0;
	virtual void OnUpdate() = 0;
	virtual void OnDestroy();
	virtual void OnSizeChanged() = 0;
	
protected:
	HWND hWnd;
	bool isRun;

	bool isInited;

private:
	void Init();
	void Release();
	bool Update();
};

