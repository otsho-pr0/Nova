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
	float x;
	float y;
	float z;
	float w;
} float4;

typedef struct
{
	float r;
	float g;
	float b;
	float a;
} Color;

namespace Renderer
{
	void* CreateVertexBuffer(void* vertices, size_t size);
	void* CreateIndexBuffer(void* indices, size_t size);
	void* CreateVertexShader(const wchar_t* path);
	void* CreatePixelShader(const wchar_t* path);
	void* CreateConstantBuffer(size_t size);
	void* CreateTexture(void* data, unsigned int width, unsigned int height, unsigned int channels);
	void* CreateTextureWithMipmapping(void* data, unsigned int width, unsigned int height, unsigned int channels);
	void* CreateInputLayout(const wchar_t* path);
	void DestroyBuffer(void*);
	void DestroyVertexShader(void*);
	void DestroyPixelShader(void*);
	void DestroyTexture(void*);
	void BindVertexBuffer(void*, unsigned int stride, unsigned int offset);
	void BindIndexBuffer(void*);
	void BindVertexShader(void*);
	void BindPixelShader(void*);
	void BindVSConstantBuffer(void*, unsigned char slot);
	void BindPSConstantBuffer(void*, unsigned char slot);
	void BindTexture(void*, unsigned char slot);
	void BindInputLayout(void*);
	void UpdateConstantBuffer(void* cBuffer, void* buffer, size_t size);
	void DrawIndexed(unsigned int numIndices);

	void DrawPoint(float2 coord, float aspect);
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