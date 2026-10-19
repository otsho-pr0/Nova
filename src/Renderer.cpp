#include <Nova/Renderer.hpp>
#include <Nova/Window.hpp>

#include <imgui_impl_dx11.h>

#include <dxgi.h>
#include <d3d11.h>
#include <d3dcompiler.h>

IDXGIFactory1* factory;
IDXGIAdapter1* adapter;
ID3D11Device* device;
ID3D11DeviceContext* context;
ID3D11InfoQueue* infoQueue;

extern void* tex;

void* Renderer::CreateVertexBuffer(void* vertices, size_t size)
{
	ID3D11Buffer* vBuffer;
	D3D11_BUFFER_DESC bd = {};
	bd.ByteWidth = size;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	D3D11_SUBRESOURCE_DATA sd = {};
	sd.pSysMem = vertices;
	device->CreateBuffer(&bd, &sd, &vBuffer);
	return vBuffer;
}

void* Renderer::CreateIndexBuffer(void* indices, size_t size)
{
	ID3D11Buffer* iBuffer;
	D3D11_BUFFER_DESC bd = {};
	bd.ByteWidth = size;
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	D3D11_SUBRESOURCE_DATA sd = {};
	sd.pSysMem = indices;
	device->CreateBuffer(&bd, &sd, &iBuffer);
	return iBuffer;
}

void* Renderer::CreateVertexShader(const wchar_t* path)
{
	ID3D11VertexShader* vs;
	ID3DBlob* blob;
	ID3DBlob* error;
	D3DCompileFromFile(path, nullptr, nullptr, "main", "vs_4_1", 0u, 0u, &blob, &error);
	if (error)
		MessageBoxA(NULL, reinterpret_cast<LPCSTR>(error->GetBufferPointer()), "Vertex Shader", MB_OK);
	device->CreateVertexShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &vs);
	blob->Release();
	return vs;
}

void* Renderer::CreatePixelShader(const wchar_t* path)
{
	ID3D11PixelShader* ps;
	ID3DBlob* blob;
	ID3DBlob* error;
	D3DCompileFromFile(path, nullptr, nullptr, "main", "ps_4_1", 0u, 0u, &blob, &error);
	if (error)
		MessageBoxA(NULL, reinterpret_cast<LPCSTR>(error->GetBufferPointer()), "Pixel Shader", MB_OK);
	device->CreatePixelShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &ps);
	blob->Release();
	return ps;
}

void* Renderer::CreateConstantBuffer(size_t size)
{
	ID3D11Buffer* cBuffer;
	D3D11_BUFFER_DESC bd = {};
	bd.ByteWidth = size;
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	device->CreateBuffer(&bd, nullptr, &cBuffer);
	return cBuffer;
}

void* Renderer::CreateTexture(void* data, unsigned int width, unsigned int height, unsigned int channels)
{
	if (data)
	{
		ID3D11Texture2D* texture;
		D3D11_TEXTURE2D_DESC td = {};
		td.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		td.ArraySize = 1u;
		td.MipLevels = 1u;
		td.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		td.SampleDesc.Count = 1u;
		td.Width = width;
		td.Height = height;
		D3D11_SUBRESOURCE_DATA sd = {};
		sd.pSysMem = data;
		sd.SysMemPitch = width * 4;
		device->CreateTexture2D(&td, &sd, &texture);

		ID3D11ShaderResourceView* srv;
		D3D11_SHADER_RESOURCE_VIEW_DESC srvd = {};
		srvd.Format = td.Format;
		srvd.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvd.Texture2D.MipLevels = 1u;
		device->CreateShaderResourceView(texture, &srvd, &srv);
		texture->Release();
		return srv;
	}
	return tex;
}

void Renderer::DestroyBuffer(void* buffer)
{
	reinterpret_cast<ID3D11Buffer*>(buffer)->Release();
}

void Renderer::DestroyVertexShader(void* vs)
{
	reinterpret_cast<ID3D11VertexShader*>(vs)->Release();
}

void Renderer::DestroyPixelShader(void* ps)
{
	reinterpret_cast<ID3D11PixelShader*>(ps)->Release();
}

void Renderer::DestroyTexture(void* srv)
{
	reinterpret_cast<ID3D11ShaderResourceView*>(srv)->Release();
}

void Renderer::BindVertexBuffer(void* vb, unsigned int stride, unsigned int offset)
{
	context->IASetVertexBuffers(0u, 1u, reinterpret_cast<ID3D11Buffer**>(&vb), &stride, &offset);
}

void Renderer::BindIndexBuffer(void* ib)
{
	context->IASetIndexBuffer(reinterpret_cast<ID3D11Buffer*>(ib), DXGI_FORMAT_R32_UINT, 0u);
}

void Renderer::BindVertexShader(void* vs)
{
	context->VSSetShader(reinterpret_cast<ID3D11VertexShader*>(vs), nullptr, 0u);
}

void Renderer::BindPixelShader(void* ps)
{
	context->PSSetShader(reinterpret_cast<ID3D11PixelShader*>(ps), nullptr, 0u);
}

void Renderer::BindVSConstantBuffer(void* cb, unsigned char slot)
{
	context->VSSetConstantBuffers(slot, 1u, reinterpret_cast<ID3D11Buffer**>(&cb));
}

void Renderer::BindPSConstantBuffer(void* cb, unsigned char slot)
{
	context->PSSetConstantBuffers(slot, 1u, reinterpret_cast<ID3D11Buffer**>(&cb));
}

void Renderer::BindTexture(void* srv, unsigned char slot)
{
	context->PSSetShaderResources(slot, 1u, reinterpret_cast<ID3D11ShaderResourceView**>(&srv));
}

void Renderer::UpdateConstantBuffer(void* cBuffer, void* buffer, size_t size)
{
	D3D11_MAPPED_SUBRESOURCE ms;
	context->Map(reinterpret_cast<ID3D11Buffer*>(cBuffer), 0u, D3D11_MAP_WRITE_DISCARD, 0u, &ms);
	std::memcpy(ms.pData, buffer, size);
	context->Unmap(reinterpret_cast<ID3D11Buffer*>(cBuffer), 0u);
}

void Renderer::DrawIndexed(unsigned int numIndices)
{
	context->DrawIndexed(numIndices, 0u, 0u);
}

RendererContext::RendererContext(Window& window) :
	mWindow(window)
{
	DXGI_SWAP_CHAIN_DESC scd = {};
	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	scd.BufferCount = 2u;
	scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	scd.SampleDesc.Count = 1u;
	scd.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	scd.Windowed = TRUE;
	scd.OutputWindow = reinterpret_cast<HWND>(mWindow.GetHandle());
	factory->CreateSwapChain(device, &scd, reinterpret_cast<IDXGISwapChain**>(&mSwapChain));

	ID3D11Texture2D* backbuffer;
	reinterpret_cast<IDXGISwapChain*>(mSwapChain)->GetBuffer(0u, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&backbuffer));
	device->CreateRenderTargetView(backbuffer, nullptr, reinterpret_cast<ID3D11RenderTargetView**>(&mRenderTargetView));
	backbuffer->Release();

	ID3D11Texture2D* depthStencil;
	D3D11_TEXTURE2D_DESC td = {};
	td.ArraySize = 1u;
	td.MipLevels = 1u;
	td.SampleDesc.Count = 1u;
	td.Width = mWindow.GetSize().x;
	td.Height = mWindow.GetSize().y;
	td.Format = DXGI_FORMAT_D32_FLOAT;
	td.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	device->CreateTexture2D(&td, nullptr, &depthStencil);
	D3D11_DEPTH_STENCIL_VIEW_DESC dsvd = {};
	dsvd.Format = td.Format;
	dsvd.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	device->CreateDepthStencilView(depthStencil, &dsvd, reinterpret_cast<ID3D11DepthStencilView**>(&mDepthStencilView));
	depthStencil->Release();

	D3D11_DEPTH_STENCIL_DESC dsd = {};
	dsd.DepthEnable = TRUE;
	dsd.DepthFunc = D3D11_COMPARISON_LESS;
	dsd.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	device->CreateDepthStencilState(&dsd, reinterpret_cast<ID3D11DepthStencilState**>(&mDepthStencilState));

	D3D11_VIEWPORT;
	mViewport = { .Width = static_cast<float>(mWindow.GetSize().x), .Height = static_cast<float>(mWindow.GetSize().y), .MinDepth = 0.0f, .MaxDepth = 1.0f};

	context->OMSetRenderTargets(1u, reinterpret_cast<ID3D11RenderTargetView**>(&mRenderTargetView), reinterpret_cast<ID3D11DepthStencilView*>(mDepthStencilView));
	context->OMSetDepthStencilState(reinterpret_cast<ID3D11DepthStencilState*>(mDepthStencilState), 0u);
	context->RSSetViewports(1u, reinterpret_cast<D3D11_VIEWPORT*>(&mViewport));

	ImGui_ImplDX11_Init(device, context);

	ID3D11InputLayout* il;
	ID3DBlob* blob;
	ID3DBlob* error;
	D3DCompileFromFile(L"../../../res/VertexShader.hlsl", nullptr, nullptr, "main", "vs_4_1", 0u, 0u, &blob, &error);
	if (error)
		MessageBoxA(NULL, reinterpret_cast<LPCSTR>(error->GetBufferPointer()), "Vertex Shader", MB_OK);

	D3D11_INPUT_ELEMENT_DESC ied[] =
	{
		{.SemanticName = "POSITION",  .Format = DXGI_FORMAT_R32G32B32_FLOAT},
		{.SemanticName = "TANGENT",   .Format = DXGI_FORMAT_R32G32B32_FLOAT, .AlignedByteOffset = sizeof(float3)},
		{.SemanticName = "BITANGENT", .Format = DXGI_FORMAT_R32G32B32_FLOAT, .AlignedByteOffset = sizeof(float3)*2},
		{.SemanticName = "NORMAL",    .Format = DXGI_FORMAT_R32G32B32_FLOAT, .AlignedByteOffset = sizeof(float3)*3},
		{.SemanticName = "TEXCOORD",  .Format = DXGI_FORMAT_R32G32_FLOAT,    .AlignedByteOffset = sizeof(float3)*4}
	};
	device->CreateInputLayout(ied, sizeof(ied) / sizeof(*ied), blob->GetBufferPointer(), blob->GetBufferSize(), &il);
	blob->Release();
	context->IASetInputLayout(il);
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	ID3D11SamplerState* ss;
	D3D11_SAMPLER_DESC sd = {};
	sd.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sd.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sd.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sd.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	device->CreateSamplerState(&sd, &ss);
	context->PSSetSamplers(0u, 1u, &ss);

	void* mVS = Renderer::CreateVertexShader(L"../../../res/VertexShader.hlsl");
	void* mPS = Renderer::CreatePixelShader(L"../../../res/PixelShader.hlsl");
	Renderer::BindVertexShader(mVS);
	Renderer::BindPixelShader(mPS);
}

RendererContext::~RendererContext()
{
	reinterpret_cast<ID3D11RenderTargetView*>(mRenderTargetView)->Release();
	reinterpret_cast<ID3D11DepthStencilView*>(mDepthStencilView)->Release();
	reinterpret_cast<ID3D11DepthStencilState*>(mDepthStencilState)->Release();
	reinterpret_cast<IDXGISwapChain*>(mSwapChain)->Release();
}

__int64 RendererContext::PollEvents(void* hWnd, unsigned int msg, unsigned long long wParam, long long lParam)
{
	switch (msg)
	{
	case WM_SIZE:
		if (mSwapChain)
		{
			unsigned int width = LOWORD(lParam);
			unsigned int height = HIWORD(lParam);

			context->OMSetRenderTargets(0u, nullptr, nullptr);
			reinterpret_cast<ID3D11RenderTargetView*>(mRenderTargetView)->Release();
			reinterpret_cast<ID3D11DepthStencilView*>(mDepthStencilView)->Release();
			reinterpret_cast<IDXGISwapChain*>(mSwapChain)->ResizeBuffers(2u, width, height, DXGI_FORMAT_UNKNOWN, 0u);

			ID3D11Texture2D* backbuffer;
			reinterpret_cast<IDXGISwapChain*>(mSwapChain)->GetBuffer(0u, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&backbuffer));
			device->CreateRenderTargetView(backbuffer, nullptr, reinterpret_cast<ID3D11RenderTargetView**>(&mRenderTargetView));
			backbuffer->Release();

			ID3D11Texture2D* depthStencil;
			D3D11_TEXTURE2D_DESC td = {};
			td.ArraySize = 1u;
			td.MipLevels = 1u;
			td.SampleDesc.Count = 1u;
			td.Width = width;
			td.Height = height;
			td.Format = DXGI_FORMAT_D32_FLOAT;
			td.BindFlags = D3D11_BIND_DEPTH_STENCIL;
			device->CreateTexture2D(&td, nullptr, &depthStencil);
			D3D11_DEPTH_STENCIL_VIEW_DESC dsvd = {};
			dsvd.Format = td.Format;
			dsvd.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
			device->CreateDepthStencilView(depthStencil, &dsvd, reinterpret_cast<ID3D11DepthStencilView**>(&mDepthStencilView));
			depthStencil->Release();

			mViewport.Width = static_cast<float>(width);
			mViewport.Height = static_cast<float>(height);

			context->OMSetRenderTargets(1u, reinterpret_cast<ID3D11RenderTargetView**>(&mRenderTargetView), reinterpret_cast<ID3D11DepthStencilView*>(mDepthStencilView));
			context->RSSetViewports(1u, reinterpret_cast<D3D11_VIEWPORT*>(&mViewport));
		}
		break;
	}

	return 0;
}

void RendererContext::Clear(const Color& color)
{
	context->ClearRenderTargetView(reinterpret_cast<ID3D11RenderTargetView*>(mRenderTargetView), reinterpret_cast<const float*>(&color));
	context->ClearDepthStencilView(reinterpret_cast<ID3D11DepthStencilView*>(mDepthStencilView), D3D11_CLEAR_DEPTH, 1.0f, 0u);
}

void RendererContext::SwapBuffers(unsigned int interval)
{
	reinterpret_cast<IDXGISwapChain*>(mSwapChain)->Present(interval, 0u);
	context->OMSetRenderTargets(1u, reinterpret_cast<ID3D11RenderTargetView**>(&mRenderTargetView), reinterpret_cast<ID3D11DepthStencilView*>(mDepthStencilView));
}

void RendererContext::SetFullscreenState(bool state)
{
	reinterpret_cast<IDXGISwapChain*>(mSwapChain)->SetFullscreenState(state, nullptr);
}

bool RendererContext::GetFullscreenState()
{
	BOOL state;
	reinterpret_cast<IDXGISwapChain*>(mSwapChain)->GetFullscreenState(&state, nullptr);
	return state;
}
