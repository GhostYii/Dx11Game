#include "Window.h"

#include <exception>

const int QUIT_CODE = 0;

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_CREATE:
	{
		//Window* window = (Window*)((LPCREATESTRUCT)lParam)->lpCreateParams;
		//SetWindowLongPtr(hWnd, GWL_USERDATA, (LONG_PTR)window);
		//window->SetHWND(hWnd);
		//window->OnCreate();
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

Window::~Window()
{
	if (hWnd)
		DestroyWindow(hWnd);
}

bool Window::IsRun()
{
	if (isRun)
		Update();

	return isRun;
}

void Window::Init()
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
		throw std::exception("Register window class failed!");

	hWnd = CreateWindowEx(WS_EX_OVERLAPPEDWINDOW, "Window", "DirectX Application", WS_CAPTION | WS_SYSMENU, CW_USEDEFAULT, CW_USEDEFAULT, 1024, 768, NULL, NULL, NULL, NULL);

	if (!hWnd)
		throw std::exception("Create window failed!");

	ShowWindow(hWnd, SW_SHOW);
	UpdateWindow(hWnd);

	isRun = true;
}

bool Window::Update()
{
	if (!this->isInited)
	{
		SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)this);
		this->OnCreate();
		this->isInited = true;
	}

	this->OnUpdate();

	MSG msg;
	while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	//Sleep(0);

	return true;
}

void Window::Release()
{
	if (hWnd && !DestroyWindow(hWnd))
		throw std::exception("Release window failed!");
}

RECT Window::GetClientWindowRect()
{
	RECT rect;
	GetClientRect(this->hWnd, &rect);
	return rect;
}

//void Window::SetHWND(HWND hWnd)
//{
//	this->hWnd = hWnd;
//}

void Window::OnDestroy()
{
	isRun = false;
}

Window::Window() : hWnd(NULL), isRun(false)
{
	Init();
}

