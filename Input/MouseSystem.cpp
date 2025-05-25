#include "pch.h"

#include "MouseSystem.h"

#include "DomWindow/DomWindow.h"

void MouseSystem::Init()
{
	PushPriority(EMousePriority::InGame);
}

void MouseSystem::UnInit()
{
	PopPriority(EMousePriority::InGame);
}

void MouseSystem::Tick(float deltaTime)
{
	
}

EMousePriority MouseSystem::GetHighestPriority() const
{
	for (i32 i = (i32)EMousePriority::COUNT - 1; i >= 0; --i)
	{
		if (bMousePriorityStack[i])
		{
			return (EMousePriority)i;
		}
	}

	DOMLOG_ERROR("No mouse priority found, defaulting to InGame");
	return EMousePriority::InGame;
}

#ifdef DOMIMGUI
void MouseSystem::DrawImGui(float deltaTime)
{
	ImGui::Text("Mouse Position: %d, %d", dmwi::GetMouseX(), dmwi::GetMouseY());
	ImGui::Text("Mouse Delta: %d, %d", dmwi::GetMouseXDelta(), dmwi::GetMouseYDelta());
	ImGui::Text("Mouse Priority: %s", ENUMSTRING(EMousePriority, GetHighestPriority()).c_str());

	// Buttons to push/pop
	for (i32 i = 0; i < (i32)EMousePriority::COUNT; ++i)
	{
		std::string buttonLabel = (bMousePriorityStack[i] ? "Pop " : "Push ") + ENUMSTRING(EMousePriority, (EMousePriority)i);
		if (ImGui::Button(buttonLabel.c_str()))
		{
			if (bMousePriorityStack[i])
			{
				PopPriority((EMousePriority)i);
			}
			else
			{
				PushPriority((EMousePriority)i);
			}
		}
	}
}
#endif