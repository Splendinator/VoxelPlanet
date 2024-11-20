#include "pch.h"

#include "ImGuiColours.h"
#include "InputContext.h"
#include "InputSystem.h"

#include "Game.h"
#include "imgui_internal.h"
#include "InputAction.h"

#include "AssetManager/AssetManager.h"

void InputSystem::Init()
{
	PushInputContext(pDefaultInputContext);
}

void InputSystem::Tick(float deltaTime)
{
	if (!pActiveInputContexts.empty())
	{
		pActiveInputContexts[pActiveInputContexts.size() - 1]->Tick();
	}
}

void InputSystem::UnInit()
{
	
}

#ifdef DOMIMGUI
void InputSystem::DrawImGui(float deltaTime)
{
	if (!pActiveInputContexts.empty())
	{
		// Draw Input Contexts
		{
			for (int i = 0; i < pActiveInputContexts.size() - 1; ++i)
			{
				std::string contextName = Game::GetAssetManager().FindNameFromObject(pActiveInputContexts[i]);
				ImGui::TextColored(ImGui::Colour::Gray, contextName.c_str()); 
			}
			std::string contextName = Game::GetAssetManager().FindNameFromObject(pActiveInputContexts.back());
			ImGui::Text(contextName.c_str()); 
		
		}

		ImGui::Separator();
		
		// Draw Input Actions
		{
			std::vector<InputActionBase*> activeInputActions = pActiveInputContexts.back()->GetInputActions();
			static std::vector<float> drawGreenTimes;
			drawGreenTimes.resize(pActiveInputContexts.size());
		
			for (int i = 0; i < activeInputActions.size(); ++i)
			{
				InputActionBase* pAction = activeInputActions[i];
			
				if (pAction->IsActive())
				{
					drawGreenTimes[i] = 1.0f;	
				}
				
				std::string actionName = Game::GetAssetManager().FindNameFromObject(pAction);
				ImVec4 colour = (ImGui::Colour::Green * drawGreenTimes[i]) + ImGui::Colour::Red * (1.0f - drawGreenTimes[i]);
				ImGui::TextColored(colour, actionName.c_str());
			}

			for (float& drawGreenTime : drawGreenTimes)
			{
				drawGreenTime -= deltaTime;
				if (drawGreenTime < 0.0f)
				{
					drawGreenTime = 0.0f;
				}
			}
		}
	}
}
#endif

void InputSystem::PushInputContext(InputContext* pInputContext)
{
	if (pInputContext == nullptr)
	{
		return;
	}
	
	pActiveInputContexts.push_back(pInputContext);

	std::sort(pActiveInputContexts.begin(), pActiveInputContexts.end(), [](InputContext* pFirst, InputContext* pSecond)
	{
		return pFirst->GetPriority() < pSecond->GetPriority();
	});
}

void InputSystem::PopInputContext(InputContext* pInputContext)
{
	if (pInputContext == nullptr)
	{
		return;
	}
	
	for (auto it = pActiveInputContexts.begin(); it != pActiveInputContexts.end(); ++it)
	{
		if (*it == pInputContext)
		{
			pActiveInputContexts.erase(it);
			return;
		}
	}

	DOMLOG_ERROR("Failed to pop input context")
}
