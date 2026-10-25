#pragma once

#include <Nova/Nova.hpp>
#include <Nova/Window.hpp>
#include <Nova/Renderer.hpp>
#include <Nova/Input.hpp>
#include <Nova/Time.hpp>
#include <Nova/Mesh.hpp>
#include <Nova/Model.hpp>
#include <Nova/Camera.hpp>
#include <string>
#include <imgui.h>

class App
{
public:
	App();

	void Run();

private:
	void Update();
	void Render();

	Window window = { L"Nova", 1034, 600 };
	RendererContext context = window;

	bool vsync = true;
	std::string controlPreview;
	unsigned char control = 0;
	std::string viewPreview;
	unsigned char view = 0;
	float sensitivity = 75.0f;
	Model model = Model("../../../res/Models/DamagedHelmet.gltf");
	//Model model = Model("../../../res/Models/Sponza.gltf");
	Mesh* mesh = Mesh::Plane();
	Camera camera;
	struct alignas(16)
	{
		int mode;
		int normalMapping;
	} pcbuffer;

	struct alignas(16)
	{
		alignas(16) Vector3 lightDir;
		alignas(16) Vector3 cameraPos;
	} vcbuffer;
	ImGuiIO& io = ImGui::GetIO();
	void* pcb = Renderer::CreateConstantBuffer(sizeof(pcbuffer));
	void* vcb = Renderer::CreateConstantBuffer(sizeof(vcbuffer));
};