#include <App.hpp>

#include <Nova/ResourceManager.hpp>

#include <imgui_impl_win32.h>
#include <imgui_impl_dx11.h>

#include <iostream>

extern IMGUI_IMPL_API __int64 ImGui_ImplWin32_WndProcHandler(struct HWND__* hWnd, unsigned int msg, unsigned long long wParam, long long lParam);

App::App()
{
	window.SetCustomWindowProc([&](void* hWnd, unsigned int msg, unsigned long long wParam, long long lParam) -> __int64
		{
			context.PollEvents(hWnd, msg, wParam, lParam);
			ImGui_ImplWin32_WndProcHandler(reinterpret_cast<HWND__*>(hWnd), msg, wParam, lParam);
			return 0;
		});

	camera.transform.position.z = -3.0f;
	camera.aspect = static_cast<float>(window.GetSize().x) / window.GetSize().y;
	camera.near = 0.01f;
	camera.far = 100.0f;
	camera.fov = Math::HalfPI / 2.0f;

	window.SetResizeCallback([&](long2 size)
		{
			camera.aspect = static_cast<float>(size.x) / size.y;
		});

	pcbuffer = { 0 };
	vcbuffer = { Vector3(0.4f, -1.5f, 0.7f).Normalize(), camera.transform.Basis() * Vector3::Forward };
	Renderer::UpdateConstantBuffer(pcb, &pcbuffer, sizeof(pcbuffer));
	Renderer::UpdateConstantBuffer(vcb, &vcbuffer, sizeof(vcbuffer));
	mesh->transform.scale = 5.0f;

	ImGui::GetStyle().ScrollbarSize = 0.0f;
}

void App::Run()
{
	while (window)
	{
		Update();
		Render();
	}
}

void App::Update()
{
	Nova::PollEvents();

	if (Input::GetKeyUp(Input::Key::Key_F11))
		context.SetFullscreenState(!context.GetFullscreenState());

	static Vector3 target = camera.transform.rotation;
	camera.transform.rotation.y = Math::LerpAngle(camera.transform.rotation.y, target.y, Time::DeltaTime() * 20.0f);
	camera.transform.rotation.x = Math::LerpAngle(camera.transform.rotation.x, target.x, Time::DeltaTime() * 20.0f);
	if (Input::GetButton(Input::Button::Right))
	{
		io.ConfigFlags |= ImGuiConfigFlags_NoMouse;
		Input::HideCursor();
		Input::ClipCursor(window);
		target.y += Input::GetDeltaX() * sensitivity * 0.0001f * ((camera.transform.Basis() * Vector3::Up).y > 0.0f ? 1.0f : -1.0f);
		target.x += Input::GetDeltaY() * sensitivity * 0.0001f;
	}
	else
	{
		io.ConfigFlags &= ~ImGuiConfigFlags_NoMouse;
		Input::ShowCursor();
		Input::FreeCursor();
	}

	if (camera.transform.rotation == target)
	{
		target.x = Math::ClampAngle(target.x);
		target.y = Math::ClampAngle(target.y);
		target.z = Math::ClampAngle(target.z);
	}

	Vector3 direction = Vector3(Input::GetAxis(Input::Key::Key_D, Input::Key::Key_Q), Input::GetAxis(Input::Key::Key_E, Input::Key::Key_A), Input::GetAxis(Input::Key::Key_Z, Input::Key::Key_S)).Normalize();
	camera.transform.position += camera.transform.Basis() * direction * Time::DeltaTime();

	ImGui_ImplWin32_NewFrame();
	ImGui_ImplDX11_NewFrame();
	ImGui::NewFrame();

	ImGui::Begin("Settings", nullptr, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBringToFrontOnFocus);
	ImGui::Text("FPS: %f", io.Framerate);
	ImGui::Checkbox("VSync", &vsync);
	ImGui::DragFloat("Sensetivity", &sensitivity, 1.0f, 10.0f, 100.0f);
	if (ImGui::DragFloat3("light dir", &vcbuffer.lightDir, 0.01f))
		vcbuffer.lightDir.Normalize();
	ImGui::SliderInt("mode", &pcbuffer.mode, 0, 3);
	vcbuffer.cameraPos = camera.transform.position;
	ImGui::End();
	ImGui::Begin("Inspector");
	if (ImGui::BeginTabBar("Transform"))
	{
		if (ImGui::BeginTabItem("Camera"))
		{
			ImGui::DragFloat3("position", &camera.transform.position, 0.01f);
			ImGui::DragFloat3("target", &target, 0.01f);
			ImGui::SliderAngle("x", &camera.transform.rotation.x);
			ImGui::SliderAngle("y", &camera.transform.rotation.y);
			ImGui::SliderAngle("z", &camera.transform.rotation.z);
			ImGui::SliderAngle("fov", &camera.fov, 0.0f);
			ImGui::DragFloat("aspect", &camera.aspect, 0.01f);
			ImGui::DragFloat("near", &camera.near, 0.01f);
			ImGui::DragFloat("far", &camera.far, 0.01f);
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("Model"))
		{
			ImGui::DragFloat3("position", &model.transform.position, 0.01f);
			ImGui::SliderAngle("x", &model.transform.rotation.x);
			ImGui::SliderAngle("y", &model.transform.rotation.y);
			ImGui::SliderAngle("z", &model.transform.rotation.z);
			ImGui::DragFloat3("scale", &model.transform.scale, 0.01f);
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("Mesh"))
		{
			ImGui::DragFloat3("position", &mesh->transform.position, 0.01f);
			ImGui::SliderAngle("x", &mesh->transform.rotation.x);
			ImGui::SliderAngle("y", &mesh->transform.rotation.y);
			ImGui::SliderAngle("z", &mesh->transform.rotation.z);
			ImGui::DragFloat3("scale", &mesh->transform.scale, 0.01f);
			ImGui::EndTabItem();
		}
		ImGui::EndTabBar();
	}
	ImGui::End();
	ImGui::Begin("Resources");
	if (ImGui::BeginTabBar("Resources"))
	{
		if (ImGui::BeginTabItem("Materials"))
		{
			static const char* preview = ResourceManager::MaterialsBegin()->first.c_str();
			if (ImGui::BeginCombo("Material", preview))
			{
				unsigned int i = 0;
				for (auto mat = ResourceManager::MaterialsBegin(); mat != ResourceManager::MaterialsEnd(); ++mat)
					if (ImGui::Selectable(mat->first.c_str()))
						preview = mat->first.c_str();
				ImGui::EndCombo();
			}
			ImGui::Image(ResourceManager::GetMaterial(preview)->GetDiffuse(), { (ImGui::GetWindowSize().x - ImGui::GetStyle().WindowPadding.x * 2.0f) / 2.0f, (ImGui::GetWindowSize().x - ImGui::GetStyle().WindowPadding.x * 2.0f) / 2.0f });
			ImGui::SameLine(0.0f, 0.0f);
			ImGui::Image(ResourceManager::GetMaterial(preview)->GetNormals(), { (ImGui::GetWindowSize().x - ImGui::GetStyle().WindowPadding.x * 2.0f) / 2.0f, (ImGui::GetWindowSize().x - ImGui::GetStyle().WindowPadding.x * 2.0f) / 2.0f });
			ImGui::Image(ResourceManager::GetMaterial(preview)->GetSpecular(), { (ImGui::GetWindowSize().x - ImGui::GetStyle().WindowPadding.x * 2.0f) / 2.0f, (ImGui::GetWindowSize().x - ImGui::GetStyle().WindowPadding.x * 2.0f) / 2.0f });
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("Meshes"))
		{
			static const char* preview = ResourceManager::MeshesBegin()->first.c_str();
			if (ImGui::BeginCombo("Mesh", preview))
			{
				unsigned int i = 0;
				for (auto mesh = ResourceManager::MeshesBegin(); mesh != ResourceManager::MeshesEnd(); ++mesh)
					if (ImGui::Selectable(mesh->first.c_str()))
						preview = mesh->first.c_str();
				ImGui::EndCombo();
			}
			ImGui::EndTabItem();
		}
		ImGui::EndTabBar();
	}
	ImGui::End();

	if (window.ShouldClose() or Input::GetKey(Input::Key::Key_Escape))
		window.Close();

	Renderer::UpdateConstantBuffer(pcb, &pcbuffer, sizeof(pcbuffer));
	Renderer::UpdateConstantBuffer(vcb, &vcbuffer, sizeof(vcbuffer));
	camera.Update();
	Renderer::BindPSConstantBuffer(pcb, 0u);
	Renderer::BindVSConstantBuffer(vcb, 1u);
}

void App::Render()
{
	context.Clear({ 0.0f, 0.0f, 0.0f, 1.0f });
	model.Draw();
	mesh->Draw();
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	context.SwapBuffers(vsync);
}