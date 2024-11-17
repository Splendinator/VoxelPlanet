#include "pch.h"

#include "GameInstance.h"
#include "GameSystem.h"

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
