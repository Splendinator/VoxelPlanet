#include "pch.h"

#include "MenuSystem.h"

#include "Input/InputAction.h"
#include "Input/InputContext.h"
#include "Input/InputSystem.h"
#include "MenuScreenBase.h"

void MenuSystem::Init()
{
	if (pOpenMenuAction)
	{
		OpenMenuDelegate.Bind(this, &MenuSystem::OpenMenu);
		pOpenMenuAction->OnActionActivated.Add(OpenMenuDelegate);
	}
	if (pCloseMenuAction)
	{
		CloseMenuDelegate.Bind(this, &MenuSystem::CloseMenu);
		pCloseMenuAction->OnActionActivated.Add(CloseMenuDelegate);
	}
}

void MenuSystem::Tick(float deltaTime)
{
	if (activeMenuScreens.size() > 0)
	{
		// Only tick the top level screen
		TInstancedAssetPtr<MenuScreenBase>* topLevelMenuScreen = activeMenuScreens.back();
		DOMASSERT(topLevelMenuScreen->IsLoaded())

		(*topLevelMenuScreen)->BaseTick(deltaTime);
	}
}

void MenuSystem::UnInit()
{
	if (bMenuOpen)
	{
		CloseMenu(OnInputActionDelegateParams());
	}
	
	if (pOpenMenuAction)
	{
		pOpenMenuAction->OnActionActivated.Remove(OpenMenuDelegate);
		delete pOpenMenuAction;
	}
	if (pCloseMenuAction)
	{
		pCloseMenuAction->OnActionActivated.Remove(CloseMenuDelegate);
		delete pCloseMenuAction;
	}
	delete pMenuInputContext;
}

void MenuSystem::PushMenuScreen(TInstancedAssetPtr<MenuScreenBase>& pMenuScreenAsset)
{
	DOMASSERT(!pMenuScreenAsset.IsLoaded())

	pMenuScreenAsset.Load();
	if (pMenuScreenAsset.IsLoaded())
	{
		activeMenuScreens.push_back(&pMenuScreenAsset);

		pMenuScreenAsset->BaseInit();
	}
}

void MenuSystem::PopMenuScreen(TInstancedAssetPtr<MenuScreenBase>& pMenuScreenAsset)
{
	DOMASSERT(!activeMenuScreens.empty())
	
	TInstancedAssetPtr<MenuScreenBase>* pOldScreen = activeMenuScreens.back();
	
	activeMenuScreens.erase(std::remove(activeMenuScreens.begin(), activeMenuScreens.end(), &pMenuScreenAsset));

	TInstancedAssetPtr<MenuScreenBase>* pNewScreen = activeMenuScreens.empty() ? nullptr : activeMenuScreens.back();

	if (pOldScreen != pNewScreen)
	{
		(*pOldScreen)->BaseUnInit();
		pOldScreen->Unload();

		if (pNewScreen)
		{
			pNewScreen->Load();

			if (pNewScreen->IsLoaded())
			{
				(*pNewScreen)->BaseInit();
			}
		}
	}
	
}

void MenuSystem::OpenMenu(OnInputActionDelegateParams Params)
{
	bMenuOpen = true;
	
	if (pInputSystem && pMenuInputContext)
	{
		pInputSystem->PushInputContext(pMenuInputContext);
	}

	PushMenuScreen(pBaseMenuScreen);
}

void MenuSystem::CloseMenu(OnInputActionDelegateParams Params)
{
	bMenuOpen = false;
	
	if (pInputSystem && pMenuInputContext)
	{
		pInputSystem->PopInputContext(pMenuInputContext);
	}

	PopMenuScreen(pBaseMenuScreen);
}
