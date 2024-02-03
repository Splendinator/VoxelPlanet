#include "pch.h"

#include "DomWindow/DomWindow.h"
#include "DomLog/DomLog.h"

#include "Game.h"
#include "Renderer.h"

#include <chrono>

#ifdef DOMIMGUI
#include <imgui.h>
#include <imgui_impl_win32.h>
#endif //~ #ifdef DOMIMGUI

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
	return dmwi::entryPoint(hInstance, hPrevInstance, pCmdLine, nCmdShow);
}

#ifdef DOMIMGUI
// Buffers for the window messages, they come in on a different thread so we try and prevent read/write collisions
std::vector<tagMSG> windowMessageBufferOne;
std::vector<tagMSG> windowMessageBufferTwo;
std::vector<tagMSG>* volatile  pCurrentWindowMessageBuffer = &windowMessageBufferOne;

void DomWindowMessageHook(tagMSG& msg)
{
	pCurrentWindowMessageBuffer->push_back(msg);
}
#endif //~ #ifdef DOMIMGUI

// Get the delta time in seconds
float GetDeltaTime()
{
	auto currentTime = std::chrono::system_clock::now();
	static auto prevTime = currentTime;

	const std::chrono::duration<float> deltaTimeDuration = currentTime - prevTime;
	prevTime = currentTime;
	return deltaTimeDuration.count();
}

bool ShouldCloseWindow()
{
	return dmwi::isPressed(dmwi::Button::ESC) && Game::CanClose();
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
	IO.DisplaySize.x = (float)dmwi::getWindowWidth();
	IO.DisplaySize.y = (float)dmwi::getWindowHeight();

	dmwi::SetMessageHook(DomWindowMessageHook);
#endif //~ #ifdef DOMIMGUI

	dmgf::Init();
	Game::Init();

	while (!ShouldCloseWindow())
	{
		float deltaTime = GetDeltaTime();

#ifdef DOMIMGUI
		std::vector<tagMSG>* pBuffer = pCurrentWindowMessageBuffer;
		pCurrentWindowMessageBuffer = (pCurrentWindowMessageBuffer == &windowMessageBufferOne) ? &windowMessageBufferTwo : &windowMessageBufferOne;
		for (tagMSG& message : *pBuffer)
		{
			ImGui_ImplWin32_WndProcHandler(message.hwnd, message.message, message.wParam, message.lParam);
		}
		pBuffer->clear();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();
#endif //~ #ifdef DOMIMGUI

		dmwi::tick();
		Game::Tick(deltaTime);
		dmgf::Tick(deltaTime);
	}

	Game::UnInit();
	dmgf::UnInit();

#ifdef DOMIMGUI
	ImGui_ImplWin32_Shutdown();
#endif //~ #ifdef DOMIMGUI

	dmwi::destroyWindow();
}
