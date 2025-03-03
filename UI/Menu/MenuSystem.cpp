#include "pch.h"

#include "MenuSystem.h"

#include "Input/InputAction.h"
#include "Input/InputContext.h"
#include "Input/InputSystem.h"
#include "Screens/MenuScreenBase.h"

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

	if (pGameStartMenuScreen.IsAssetSet())
	{
		PushMenuScreen(pGameStartMenuScreen);
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

	while (activeMenuScreens.size() > 0)
	{
		DOMASSERT(activeMenuScreens.back() != nullptr)
		PopMenuScreen(*activeMenuScreens.back());
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
		if (activeMenuScreens.size() > 0)
		{
			TInstancedAssetPtr<MenuScreenBase>* pOldScreen = activeMenuScreens.back();
			(*pOldScreen)->BaseUnInit();
		}

		activeMenuScreens.push_back(&pMenuScreenAsset);
		pMenuScreenAsset->BaseInit(this);
	}
}

void MenuSystem::PopMenuScreen(TInstancedAssetPtr<MenuScreenBase>& pMenuScreenAsset)
{
	DOMASSERT(!activeMenuScreens.empty())
	
	TInstancedAssetPtr<MenuScreenBase>* pOldScreen = activeMenuScreens.back();
	if (&pMenuScreenAsset == pOldScreen)
	{
		pMenuScreenAsset->BaseUnInit();
	}
	pOldScreen->Unload();
	
	activeMenuScreens.erase(std::remove(activeMenuScreens.begin(), activeMenuScreens.end(), &pMenuScreenAsset));

	if (activeMenuScreens.size() > 0)
	{
		TInstancedAssetPtr<MenuScreenBase>* pNewScreen = activeMenuScreens.back();
		if (pNewScreen != pOldScreen)
		{
			(*pNewScreen)->BaseInit(this);
		}
	}
	
}

void MenuSystem::CloseScreen(MenuScreenBase* pMenuScreen)
{
	for (TInstancedAssetPtr<MenuScreenBase>* pActiveMenuScreen : activeMenuScreens)
	{
		if (pActiveMenuScreen->Get() == pMenuScreen)
		{
			PopMenuScreen(*pActiveMenuScreen);
			return;
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
