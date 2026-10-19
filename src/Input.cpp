#include <Nova/Input.hpp>
#include <Nova/Window.hpp>
#include <Nova/Windows.hpp>

unsigned char keyboard[256];
unsigned char mouse[3];
long mouseDeltaX;
long mouseDeltaY;

void Input_Reset()
{
	for (auto& key : keyboard)
		key = 0u;
	for (auto& button : mouse)
		button = 0u;
}

bool Input::GetKey(Key key)
{
	return keyboard[static_cast<unsigned char>(key)];
}

bool Input::GetKeyDown(Key key)
{
	return keyboard[static_cast<unsigned char>(key)] == 0b111100;
}

bool Input::GetKeyUp(Key key)
{
	return keyboard[static_cast<unsigned char>(key)] == 0b000011;
}

bool Input::GetButton(Button button)
{
	return mouse[static_cast<unsigned char>(button)];
}

bool Input::GetButtonDown(Button button)
{
	return mouse[static_cast<unsigned char>(button)] == 0b111100;
}

bool Input::GetButtonUp(Button button)
{
	return mouse[static_cast<unsigned char>(button)] == 0b000011;
}

long Input::GetAxis(Key positive, Key negative)
{
	return Input::GetKey(positive) ? 1.0f : Input::GetKey(negative) ? -1.0f : 0.0f;
}

long Input::GetDeltaX()
{
	return mouseDeltaX;
}

long Input::GetDeltaY()
{
	return mouseDeltaY;
}

bool Input::IsCursorClipped()
{
	RECT rect;
	GetClipCursor(&rect);

	int screenLeft = GetSystemMetrics(SM_XVIRTUALSCREEN);
	int screenTop = GetSystemMetrics(SM_YVIRTUALSCREEN);
	int screenWidth = GetSystemMetrics(SM_CXVIRTUALSCREEN);
	int screenHeight = GetSystemMetrics(SM_CYVIRTUALSCREEN);

	return !(rect.left == screenLeft && rect.top == screenTop && (rect.right - rect.left) == screenWidth && (rect.bottom - rect.top) == screenHeight);
}

void Input::ClipCursor(Window& window)
{
	RECT rect;
	GetClientRect(reinterpret_cast<HWND>(window.GetHandle()), &rect);
	MapWindowPoints(reinterpret_cast<HWND>(window.GetHandle()), NULL, reinterpret_cast<POINT*>(&rect), 2u);
	::ClipCursor(&rect);
}

void Input::FreeCursor()
{
	::ClipCursor(nullptr);
}

void Input::ShowCursor()
{
	while (::ShowCursor(TRUE) < 0);
}

void Input::HideCursor()
{
	while (::ShowCursor(FALSE) >= 0);
}