#include "../Graphics/DomGraphics.h"

#include "DomWindow/DomWindow.h"
#include "DomLog/DomLog.h"

#include <chrono>

#ifdef DOMIMGUI
#include <imgui.h>
#include <imgui_impl_win32.h>
#endif //~ #ifdef DOMIMGUI

#include "Game.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
	return dmwi::entryPoint(hInstance, hPrevInstance, pCmdLine, nCmdShow);
}

std::vector<tagMSG> messages;

void DomWindowMessageHook(tagMSG& msg)
{
	ImGui_ImplWin32_WndProcHandler(msg.hwnd, msg.message, msg.wParam, msg.lParam);
}

// Get the delta time in seconds
float GetDeltaTime()
{
	static auto prevTime = std::chrono::system_clock::now().time_since_epoch();
	auto currentTime = std::chrono::system_clock::now().time_since_epoch();
	
	const float deltaTime = (currentTime - prevTime).count() / 10000000.f; // TODO: This magic number should be gathered from the std::ratio (Numberator / Denominator)s

	prevTime = currentTime;

	return deltaTime;
}

void domMain()
{
	dmwi::createWindow({ "Roguelike", 1792, 1008 });
	dmlg::initConsole();

#ifdef DOMIMGUI
	if (ImGuiContext* context = ImGui::CreateContext())
	{
		ImGui::SetCurrentContext(context);
		ImGui::StyleColorsDark();
	}
	else
	{
		DOMLOG_WARN("ImGui not working!");
	}

	HWND windowHandle = dmwi::getWindowHandle();
	ImGui_ImplWin32_Init(windowHandle);
	ImGuiIO& IO = ImGui::GetIO();
	IO.DisplaySize.x = dmwi::getWindowWidth();
	IO.DisplaySize.y = dmwi::getWindowHeight();

	dmwi::SetMessageHook(DomWindowMessageHook);
#endif //~ #ifdef DOMIMGUI

	dmgf::NewInit();
	game::init(48,27);

	while (!dmwi::isPressed(dmwi::Button::ESC))
	{
		float deltaTime = GetDeltaTime();

		if (dmwi::isPressed(dmwi::Button::F1))
		{
			static bool bIsCursorLocked;
			dmwi::LockCursor(bIsCursorLocked = !bIsCursorLocked);
		}

#ifdef DOMIMGUI
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();
#endif //~ #ifdef DOMIMGUI

		dmwi::tick();
		game::tick(deltaTime);
		dmgf::Tick(deltaTime);
	}

	dmgf::UnInit();

#ifdef DOMIMGUI
	ImGui_ImplWin32_Shutdown();
#endif //~ #ifdef DOMIMGUI

	dmwi::destroyWindow();
}