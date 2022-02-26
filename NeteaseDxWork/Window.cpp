#include "Window.h"
#include <Keyboard.h>
#include <Mouse.h>
#include <exception>

#include "imgui_impl_win32.h"
#include "internal_resource.h"

const int QUIT_CODE = 0;

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{		
	ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam);
	switch (msg)
	{
	case WM_ACTIVATE:
	case WM_ACTIVATEAPP:
	{
		DirectX::Mouse::ProcessMessage(msg, wParam, lParam);
	}
	case WM_SYSKEYDOWN:
	case WM_KEYDOWN:
	case WM_KEYUP:
	case WM_SYSKEYUP:
	{
		DirectX::Keyboard::ProcessMessage(msg, wParam, lParam);
		break;
	}
	case WM_INPUT:
	case WM_MOUSEMOVE:
	case WM_LBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_RBUTTONDOWN:
	case WM_RBUTTONUP:
	case WM_MBUTTONDOWN:
	case WM_MBUTTONUP:
	case WM_MOUSEWHEEL:
	case WM_XBUTTONDOWN:
	case WM_XBUTTONUP:
	case WM_MOUSEHOVER:
	{
		DirectX::Mouse::ProcessMessage(msg, wParam, lParam);
		break; 
	}
	case WM_MOUSEACTIVATE:
	{
		return MA_ACTIVATEANDEAT;
	}
	case WM_CREATE:
	{
		//Window* window = (Window*)((LPCREATESTRUCT)lParam)->lpCreateParams;
		//SetWindowLongPtr(hWnd, GWL_USERDATA, (LONG_PTR)window);
		//window->SetHWND(hWnd);
		//window->OnCreate();
		break;
	}		
	case WM_SIZE:
	{
		Window* pWindow = (Window*)GetWindowLongPtr(hWnd, GWL_USERDATA);
		if (pWindow)
			pWindow->OnSizeChanged();
		break;
	}case WM_DESTROY:
	{
		Window* pWindow = (Window*)GetWindowLongPtr(hWnd, GWL_USERDATA);
		if (pWindow)
			pWindow->OnDestroy();
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
	wc.hIcon = static_cast<HICON>(LoadImage(GetModuleHandle(nullptr), MAKEINTRESOURCE(IDI_ICON1), IMAGE_ICON, 128, 128, 0));
	wc.hIconSm = static_cast<HICON>(LoadImage(GetModuleHandle(nullptr), MAKEINTRESOURCE(IDI_ICON1), IMAGE_ICON, 128, 128, 0));
	wc.hInstance = NULL;
	wc.lpszClassName = "Window";
	wc.lpszMenuName = "";
	wc.style = NULL;
	wc.lpfnWndProc = &WndProc;

	if (!RegisterClassEx(&wc))
		throw std::exception("Register window class failed!");

	hWnd = CreateWindowEx(WS_EX_OVERLAPPEDWINDOW, "Window", "To the Moon", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 1024, 768, NULL, NULL, NULL, NULL);

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
	while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	{
		if (msg.message == WM_QUIT)
			return false;

		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	

	//Sleep(1);

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
	ClientToScreen(hWnd, (LPPOINT)&rect.left);
	ClientToScreen(hWnd, (LPPOINT)&rect.right);
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

