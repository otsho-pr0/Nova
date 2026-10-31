#include <Nova/Window.hpp>
#include <Nova/Windows.hpp>

extern unsigned char keyboard[256];
extern unsigned char mouse[3];
extern long mouseDeltaX;
extern long mouseDeltaY;
extern void Input_Reset();

struct Window::Data
{
	HWND mHWnd;
	bool mShouldClose;
};

__int64 __stdcall WindowProc(void* hWnd, unsigned int msg, unsigned __int64 wParam, __int64 lParam)
{
	Window* window = reinterpret_cast<Window*>(GetWindowLongPtrW(reinterpret_cast<HWND>(hWnd), GWLP_USERDATA));

	if (window)
		if (window->CustomWindowProc)
			window->CustomWindowProc(hWnd, msg, wParam, lParam);

	switch (msg)
	{
	case WM_NCCREATE:
	{
		CREATESTRUCTW* cs = reinterpret_cast<CREATESTRUCTW*>(lParam);
		window = reinterpret_cast<Window*>(cs->lpCreateParams);
		SetWindowLongPtrW(reinterpret_cast<HWND>(hWnd), GWLP_USERDATA, reinterpret_cast<LONG_PTR>(window));
	} break;
	case WM_CLOSE:
	{
		window->data->mShouldClose = true;
		return 0;
	} break;
	case WM_SIZE:
	{
		Input_Reset();
		long width = LOWORD(lParam);
		long height = HIWORD(lParam);
		if (window->ResizeCallback)
			window->ResizeCallback({ width, height });
	} break;
	case WM_ACTIVATE:
	case WM_KILLFOCUS:
		Input_Reset();
		break;
	case WM_INPUT:
	{
		UINT size = sizeof(RAWINPUT);
		RAWINPUT ri = {};
		GetRawInputData(reinterpret_cast<HRAWINPUT>(lParam), RID_INPUT, &ri, &size, sizeof(RAWINPUTHEADER));
		if (ri.header.dwType == RIM_TYPEMOUSE)
		{
			mouseDeltaX = ri.data.mouse.lLastX;
			mouseDeltaY = ri.data.mouse.lLastY;
		}
	} break;
	case WM_KEYDOWN:
		if (keyboard[wParam] ^ 0b110000)
			keyboard[wParam] = 0b111100;
		break;
	case WM_KEYUP:
		keyboard[wParam] = 0b000011;
		break;
	case WM_LBUTTONDOWN:
		if (mouse[0] ^ 0b110000)
			mouse[0] = 0b111100;
		break;
	case WM_LBUTTONUP:
		mouse[0] = 0b000011;
		break;
	case WM_MBUTTONDOWN:
		if (mouse[1] ^ 0b110000)
			mouse[1] = 0b111100;
		break;
	case WM_MBUTTONUP:
		mouse[1] = 0b000011;
		break;
	case WM_RBUTTONDOWN:
		if (mouse[2] ^ 0b110000)
			mouse[2] = 0b111100;
		break;
	case WM_RBUTTONUP:
		mouse[2] = 0b000011;
		break;
	}
	return DefWindowProcW(reinterpret_cast<HWND>(hWnd), msg, wParam, lParam);
}

WNDCLASSW wc = { .lpfnWndProc = reinterpret_cast<WNDPROC>(WindowProc), .hInstance = GetModuleHandleW(nullptr), .lpszClassName = L"Window Class" };

Window::Window(const wchar_t* title, long width, long height) :
	data(new Window::Data(nullptr, false))
{
	RECT window = { .left = 0, .top = 0, .right = width, .bottom = height };
	AdjustWindowRect(&window, WS_OVERLAPPEDWINDOW, FALSE);
	data->mHWnd = CreateWindowW(wc.lpszClassName, title, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, window.right - window.left, window.bottom - window.top, nullptr, nullptr, wc.hInstance, this);
	ShowWindow(data->mHWnd, SW_SHOW);
	RAWINPUTDEVICE rid = {};
	rid.usUsagePage = 0x01;
	rid.usUsage = 0x02;
	rid.hwndTarget = data->mHWnd;
	RegisterRawInputDevices(&rid, 1u, sizeof(rid));
}

Window::~Window()
{
	delete data;
}

Window::operator bool()
{
	return data->mHWnd;
}

void Window::Close()
{
	data->mShouldClose = true;
	DestroyWindow(data->mHWnd);
	data->mHWnd = nullptr;
}

void Window::Focus()
{
}

bool Window::Active()
{
	return GetActiveWindow() == data->mHWnd;
}

long2 Window::GetSize()
{
	RECT rect;
	GetClientRect(data->mHWnd, &rect);
	return { rect.right - rect.left, rect.bottom - rect.top };
}

bool& Window::ShouldClose()
{
	return data->mShouldClose;
}

void* Window::GetHandle()
{
	return data->mHWnd;
}

void Window::SetCustomWindowProc(std::function<__int64(void* hWnd, unsigned int msg, unsigned __int64 wParam, __int64 lParam)> proc)
{
	CustomWindowProc = proc;
}

void Window::SetResizeCallback(std::function<void(long2 size)> callback)
{
	ResizeCallback = callback;
}
