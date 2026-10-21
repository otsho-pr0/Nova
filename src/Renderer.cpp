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
	{
		MessageBoxA(NULL, reinterpret_cast<LPCSTR>(error->GetBufferPointer()), "Vertex Shader", MB_OK);
		error->Release();
	}
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
	{
		MessageBoxA(NULL, reinterpret_cast<LPCSTR>(error->GetBufferPointer()), "Pixel Shader", MB_OK);
		error->Release();
	}
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

void* Renderer::CreateTextureWithMipmapping(void* data, unsigned int width, unsigned int height, unsigned int channels)
{
	if (data)
	{
		ID3D11Texture2D* texture;
		D3D11_TEXTURE2D_DESC td = {};
		td.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
		td.ArraySize = 1u;
		td.MipLevels = 0u;
		td.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		td.SampleDesc.Count = 1u;
		td.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;
		td.Width = width;
		td.Height = height;
		device->CreateTexture2D(&td, nullptr, &texture);
		context->UpdateSubresource(texture, 0u, nullptr, data, width * 4, 0u);

		ID3D11ShaderResourceView* srv;
		D3D11_SHADER_RESOURCE_VIEW_DESC srvd = {};
		srvd.Format = td.Format;
		srvd.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvd.Texture2D.MipLevels = -1u;
		device->CreateShaderResourceView(texture, &srvd, &srv);
		context->GenerateMips(srv);
		texture->Release();
		return srv;
	}
	return tex;
}

void* Renderer::CreateInputLayout(const wchar_t* path)
{
	ID3D11InputLayout* il;
	ID3D11ShaderReflection* sr;
	ID3DBlob* blob;
	ID3DBlob* error;
	D3DCompileFromFile(path, nullptr, nullptr, "main", "vs_4_1", 0u, 0u, &blob, &error);
	if (error)
	{
		MessageBoxA(NULL, reinterpret_cast<LPCSTR>(error->GetBufferPointer()), "Vertex Shader", MB_OK);
		error->Release();
	}
	D3DReflect(blob->GetBufferPointer(), blob->GetBufferSize(), __uuidof(ID3D11ShaderReflection), reinterpret_cast<void**>(&sr));

	UINT numElements = 0u;
	D3D11_SIGNATURE_PARAMETER_DESC spd;
	while (SUCCEEDED(sr->GetInputParameterDesc(numElements, &spd)))
		numElements++;

	D3D11_INPUT_ELEMENT_DESC* ied = new D3D11_INPUT_ELEMENT_DESC[numElements]{};

	for (unsigned int i = 0u; i < numElements; i++)
	{
		static UINT offset = 0u;
		D3D11_SIGNATURE_PARAMETER_DESC spd;
		sr->GetInputParameterDesc(i, &spd);
		ied[i].SemanticName = spd.SemanticName;
		ied[i].SemanticIndex = spd.SemanticIndex;
		ied[i].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		ied[i].AlignedByteOffset = offset;

		switch (spd.Mask)
		{
		case 0b000001:
			ied[i].Format = DXGI_FORMAT_R32_FLOAT;
			offset += sizeof(float);
			break;
		case 0b000011:
			ied[i].Format = DXGI_FORMAT_R32G32_FLOAT;
			offset += sizeof(float2);
			break;
		case 0b000111:
			ied[i].Format = DXGI_FORMAT_R32G32B32_FLOAT;
			offset += sizeof(float3);
			break;
		case 0b001111:
			ied[i].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
			offset += sizeof(float4);
			break;
		}
	}

	device->CreateInputLayout(ied, numElements, blob->GetBufferPointer(), blob->GetBufferSize(), &il);
	delete[] ied;
	blob->Release();
	sr->Release();
	return il;
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

void Renderer::BindInputLayout(void* il)
{
	context->IASetInputLayout(reinterpret_cast<ID3D11InputLayout*>(il));
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

void Renderer::DrawPoint(float2 coord, float aspect)
{
	static void* vs = Renderer::CreateVertexShader(L"../../../res/VertexDebug.hlsl");
	static void* ps = Renderer::CreatePixelShader(L"../../../res/PixelDebug.hlsl");
	ID3D11VertexShader* pVS;
	ID3D11PixelShader* pPS;
	context->VSGetShader(&pVS, nullptr, nullptr);
	context->PSGetShader(&pPS, nullptr, nullptr);
	ID3D11InputLayout* il;
	context->IAGetInputLayout(&il);
	context->IASetInputLayout(nullptr);

	struct alignas(16)
	{
		float2 coord;
		float aspect;
	} cbuffer = { coord, aspect };
	static void* cb = Renderer::CreateConstantBuffer(sizeof(cbuffer));
	Renderer::UpdateConstantBuffer(cb, &cbuffer, sizeof(cbuffer));

	Renderer::BindVertexShader(vs);
	Renderer::BindPixelShader(ps);
	Renderer::BindVSConstantBuffer(cb, 0u);
	context->Draw(6u, 0);
	context->IASetInputLayout(il);
	context->VSSetShader(pVS, nullptr, 0u);
	context->PSSetShader(pPS, nullptr, 0u);
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

	void* il = Renderer::CreateInputLayout(L"../../../res/VertexShader.hlsl");
	Renderer::BindInputLayout(il);
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	ID3D11SamplerState* ss;
	D3D11_SAMPLER_DESC sd = {};
	sd.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sd.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sd.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sd.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sd.MaxLOD = D3D11_FLOAT32_MAX;
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
