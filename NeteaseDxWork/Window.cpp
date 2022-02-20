#include "Window.h"

Window* pWindow = nullptr;
const int QUIT_CODE = 0;

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_CREATE:
	{
		Window* window = (Window*)((LPCREATESTRUCT)lParam)->lpCreateParams;
		SetWindowLongPtr(hWnd, GWL_USERDATA, (LONG_PTR)window);
		window->SetHWND(hWnd);
		window->OnCreate();
		break;
	}
	case WM_DESTROY:
	{
		Window* window = (Window*)GetWindowLong(hWnd, GWL_USERDATA);
		window->OnDestroy();
		//pWindow->OnDestroy();
		PostQuitMessage(QUIT_CODE);
		break;
	}
	default:
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}

	return NULL;
}

bool Window::IsRun()
{
	return isRun;
}

bool Window::Init()
{
	WNDCLASSEX wc;
	wc.cbClsExtra = NULL;
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.cbWndExtra = NULL;
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	wc.hInstance = NULL;
	wc.lpszClassName = "Window";
	wc.lpszMenuName = "";
	wc.style = NULL;
	wc.lpfnWndProc = WndProc;

	if (!RegisterClassEx(&wc))
		return false;

	if (!pWindow)
		pWindow = this;

	hWnd = CreateWindowEx(WS_EX_OVERLAPPEDWINDOW, "Window", "DirectX Application", WS_CAPTION | WS_SYSMENU, CW_USEDEFAULT, CW_USEDEFAULT, 1024, 768, NULL, NULL, NULL, this);

	if (!hWnd)
		return false;

	ShowWindow(hWnd, SW_SHOW);
	UpdateWindow(hWnd);

	isRun = true;

	return true;
}

bool Window::Broadcast()
{
	pWindow->OnUpdate();

	MSG msg;
	while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	//Sleep(0);

	return true;
}

bool Window::Release()
{
	if (hWnd && !DestroyWindow(hWnd))
		return false;

	return true;
}

RECT Window::GetClientWindowRect()
{
	RECT rect;
	GetClientRect(this->hWnd, &rect);
	return rect;
}

void Window::SetHWND(HWND hWnd)
{
	this->hWnd = hWnd;
}

void Window::OnDestroy()
{
	isRun = false;
}

Window::Window() : hWnd(NULL), isRun(false)
{
}

