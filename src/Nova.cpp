#include <Nova/Nova.hpp>
#include <Nova/Window.hpp>
#include <Nova/Windows.hpp>
#include <Nova/ResourceManager.hpp>
#include <Nova/Renderer.hpp>

#include <dxgi.h>
#include <d3d11.h>

#include <stb_image.h>

#include <chrono>

extern unsigned char keyboard[256];
extern unsigned char mouse[3];
extern long mouseDeltaX;
extern long mouseDeltaY;
extern WNDCLASSW wc;

extern IDXGIFactory1* factory;
extern IDXGIAdapter1* adapter;
extern ID3D11Device* device;
extern ID3D11DeviceContext* context;
extern ID3D11InfoQueue* infoQueue;

extern std::chrono::high_resolution_clock::time_point start;
extern std::chrono::duration<int, std::milli> deltaTime;

Material* mat;
void* tex;

void Nova::Init()
{
	RegisterClassW(&wc);

	CreateDXGIFactory1(__uuidof(IDXGIFactory1), reinterpret_cast<void**>(&factory));

	IDXGIAdapter1* a;
	UINT i = 0;
	while (SUCCEEDED(factory->EnumAdapters1(i, &a)))
	{
		DXGI_ADAPTER_DESC1 ad;
		a->GetDesc1(&ad);
		//std::cout << i << ": ";
		//std::wcout << ad.Description << std::endl;
		//std::wcout << "Video Memory: " << ad.DedicatedVideoMemory / 1024 / 1024 << "MB" << std::endl;
		//std::wcout << std::endl;
		i++;
		a->Release();
	}

	factory->EnumAdapters1(0, &a);
	adapter = a;

	D3D11CreateDevice(adapter, D3D_DRIVER_TYPE_UNKNOWN, NULL, D3D11_CREATE_DEVICE_DEBUG, nullptr, 0u, D3D11_SDK_VERSION, &device, nullptr, &context);
	device->QueryInterface(__uuidof(ID3D11InfoQueue), reinterpret_cast<void**>(&infoQueue));

	int width, height, channels;
	void* data = stbi_load("../../../res/TextureLess.jpg", &width, &height, &channels, 4);
	tex = Renderer::CreateTextureWithMipmapping(data, width, height, channels);
	stbi_image_free(data);
	mat = ResourceManager::CreateMaterial("default", "../../../res/Checker.jpg", "../../../res/CheckerNormals.jpg", "");

	Mesh::mCB = Renderer::CreateConstantBuffer(sizeof(Matrix) * 3);
}

void Nova::PollEvents()
{
	deltaTime = std::chrono::duration_cast<std::chrono::duration<int, std::milli>>(std::chrono::high_resolution_clock::now() - start);
	start = std::chrono::high_resolution_clock::now();

	for (auto& key : keyboard)
		key &= 0b110000;
	for (auto& button : mouse)
		button &= 0b110000;

	mouseDeltaX = 0;
	mouseDeltaY = 0;

	MSG msg;
	while (PeekMessageW(&msg, NULL, 0u, 0u, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessageW(&msg);
	}

	UINT storedMessages = reinterpret_cast<ID3D11InfoQueue*>(infoQueue)->GetNumStoredMessages();
	for (unsigned int i = 0; i < storedMessages; i++)
	{
		SIZE_T length;
		reinterpret_cast<ID3D11InfoQueue*>(infoQueue)->GetMessageW(i, nullptr, &length);
		D3D11_MESSAGE* message = reinterpret_cast<D3D11_MESSAGE*>(new unsigned char[length]);
		reinterpret_cast<ID3D11InfoQueue*>(infoQueue)->GetMessageW(i, message, &length);
		MessageBoxA(NULL, message->pDescription, "DirectX Error", MB_OK | MB_ICONERROR);
		delete[] message;
	}
	reinterpret_cast<ID3D11InfoQueue*>(infoQueue)->ClearStoredMessages();
}

void Nova::Destroy()
{
	infoQueue->Release();
	context->Release();
	device->Release();
	UnregisterClassW(wc.lpszClassName, wc.hInstance);
}