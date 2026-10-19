#pragma once

class Window;

typedef struct
{
	float x;
	float y;
} float2;

typedef struct
{
	float x;
	float y;
	float z;
} float3;

typedef struct
{
	float r;
	float g;
	float b;
	float a;
} Color;

class Renderer
{
public:
	static void* CreateVertexBuffer(void* vertices, size_t size);
	static void* CreateIndexBuffer(void* indices, size_t size);
	static void* CreateVertexShader(const wchar_t* path);
	static void* CreatePixelShader(const wchar_t* path);
	static void* CreateConstantBuffer(size_t size);
	static void* CreateTexture(void* data, unsigned int width, unsigned int height, unsigned int channels);
	static void DestroyBuffer(void*);
	static void DestroyVertexShader(void*);
	static void DestroyPixelShader(void*);
	static void DestroyTexture(void*);
	static void BindVertexBuffer(void*, unsigned int stride, unsigned int offset);
	static void BindIndexBuffer(void*);
	static void BindVertexShader(void*);
	static void BindPixelShader(void*);
	static void BindVSConstantBuffer(void*, unsigned char slot);
	static void BindPSConstantBuffer(void*, unsigned char slot);
	static void BindTexture(void*, unsigned char slot);
	static void UpdateConstantBuffer(void* cBuffer, void* buffer, size_t size);
	static void DrawIndexed(unsigned int numIndices);
};

class RendererContext
{
public:
	RendererContext(Window&);
	RendererContext(RendererContext&) = delete;
	~RendererContext();

	RendererContext& operator=(RendererContext&) = delete;

	__int64 PollEvents(void*, unsigned int, unsigned long long, long long);

	void Clear(const Color&);
	void SwapBuffers(unsigned int interval = 0u);

	void SetFullscreenState(bool state);
	bool GetFullscreenState();

private:
	Window& mWindow;
	void* mSwapChain;
	void* mRenderTargetView;
	void* mDepthStencilView;
	void* mDepthStencilState;
	struct
	{
		float TopLeftX;
		float TopLeftY;
		float Width;
		float Height;
		float MinDepth;
		float MaxDepth;
	} mViewport;
};