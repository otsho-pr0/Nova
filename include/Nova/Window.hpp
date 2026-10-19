#pragma once

#include <functional>

typedef struct
{
	long x;
	long y;
} long2;

class Window
{
public:
	Window(const wchar_t* title, long width, long height);
	Window(const Window&) = delete;
	~Window();

	Window& operator=(const Window&) = delete;
	operator bool();

	void Close();
	void Focus();

	long2 GetSize();
	bool& ShouldClose();
	bool Active();
	void* GetHandle();

	void SetCustomWindowProc(std::function<__int64(void* hWnd, unsigned int msg, unsigned __int64 wParam, __int64 lParam)>);
	void SetResizeCallback(std::function<void(long2 size)>);

private:
	struct Data;

	std::function<__int64(void* hWnd, unsigned int msg, unsigned __int64 wParam, __int64 lParam)> CustomWindowProc = nullptr;
	std::function<void(long2 size)> ResizeCallback = nullptr;

	friend __int64 __stdcall WindowProc(void* hWnd, unsigned int msg, unsigned __int64 wParam, __int64 lParam);

	Data* data;
};