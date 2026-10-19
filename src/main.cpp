#include <App.hpp>
#include <imgui.h>

int main()
{
	Nova::Init();
	ImGui::CreateContext();
	App app;
	app.Run();
	Nova::Destroy();
}