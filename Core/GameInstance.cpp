#include "pch.h"

#include "GameInstance.h"

#include "Game.h"
#include "GameSystem.h"

#include "AssetManager/AssetManager.h"

#include "DomWindow/DomWindow.h"

void GameInstance::InitGameSystems()
{
	for (GameSystem* pGameSystem : pGameSystems)
	{
		if (pGameSystem)
		{
			pGameSystem->Init();
		}
	}
}

void GameInstance::TickGameSystems(float deltaTime)
{
	for (GameSystem* pGameSystem : pGameSystems)
	{
		if (pGameSystem)
		{
			pGameSystem->Tick(deltaTime);
		}
	}
}

void GameInstance::UnInitGameSystems()
{
	// Uninit in reverse order
	for (int i = (int)pGameSystems.size() - 1; i >= 0; --i)
	{
		if (pGameSystems[i])
		{
			pGameSystems[i]->UnInit();
		}
	}
}

#ifdef DOMIMGUI
void GameInstance::TickImGui(float deltaTime)
{
	if (dmwi::isPressed(dmwi::F8))
	{
		bShowingImGui = !bShowingImGui;
	}
	
	if (bShowingImGui)
	{
		ImGui::Begin("Debug");
		
		// Needed because std::vector<bool> is bitflags and doesn't fucking work
		struct BoolWrapper
		{
			bool bBool = false;
		};
		static std::vector<BoolWrapper> bDrawGameSystemDebug;
		bDrawGameSystemDebug.resize(pGameSystems.size());
		
		for (int i = 0; i < pGameSystems.size(); ++i)
		{
			if (pGameSystems[i])
			{
				std::string name = Game::GetAssetManager().FindNameFromObject(pGameSystems[i]);
				ImGui::Checkbox(name.c_str(), &bDrawGameSystemDebug[i].bBool);
				if (bDrawGameSystemDebug[i].bBool)
				{
					ImGui::Begin(name.c_str());
					pGameSystems[i]->DrawImGui(deltaTime);
					ImGui::End();
				}
			}
		}
		ImGui::End();
	}
}
#endif
