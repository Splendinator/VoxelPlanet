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
		pOpenMenuAction->onActionActivatedDelegates.Add(OpenMenuDelegate);
	}
	if (pCloseMenuAction)
	{
		CloseMenuDelegate.Bind(this, &MenuSystem::CloseMenu);
		pCloseMenuAction->onActionActivatedDelegates.Add(CloseMenuDelegate);
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
		InstancedAssetPtr<MenuScreenBase>* topLevelMenuScreen = activeMenuScreens.back();
		DOMASSERT(topLevelMenuScreen->IsLoaded())

		(*topLevelMenuScreen)->BaseTick(deltaTime);
	}
}

void MenuSystem::UnInit()
{
	if (bMainMenuOpen)
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
		pOpenMenuAction->onActionActivatedDelegates.Remove(OpenMenuDelegate);
	}
	if (pCloseMenuAction)
	{
		pCloseMenuAction->onActionActivatedDelegates.Remove(CloseMenuDelegate);
	}
}

void MenuSystem::PushMenuScreen(InstancedAssetPtr<MenuScreenBase>& pMenuScreenAsset)
{
	DOMASSERT(!pMenuScreenAsset.IsLoaded())

	pMenuScreenAsset.Load();
	if (pMenuScreenAsset.IsLoaded())
	{
		if (activeMenuScreens.size() > 0)
		{
			InstancedAssetPtr<MenuScreenBase>* pOldScreen = activeMenuScreens.back();
			(*pOldScreen)->BaseUnInit();
		}

		activeMenuScreens.push_back(&pMenuScreenAsset);
		pMenuScreenAsset->BaseInit(this);
	}
}

void MenuSystem::PopMenuScreen(InstancedAssetPtr<MenuScreenBase>& pMenuScreenAsset)
{
	// #TODO: There's a crash here, I think when you hit ESC in the class select screen so disable it there using inputcontext_frontend maybe
	DOMASSERT(!activeMenuScreens.empty()) 
	
	InstancedAssetPtr<MenuScreenBase>* pOldScreen = activeMenuScreens.back();
	if (&pMenuScreenAsset == pOldScreen)
	{
		pMenuScreenAsset->BaseUnInit();
	}
	pOldScreen->Unload();
	
	activeMenuScreens.erase(std::remove(activeMenuScreens.begin(), activeMenuScreens.end(), &pMenuScreenAsset));

	if (activeMenuScreens.size() > 0)
	{
		InstancedAssetPtr<MenuScreenBase>* pNewScreen = activeMenuScreens.back();
		if (pNewScreen != pOldScreen)
		{
			(*pNewScreen)->BaseInit(this);
		}
	}
	
}

void MenuSystem::CloseScreen(MenuScreenBase* pMenuScreen)
{
	for (InstancedAssetPtr<MenuScreenBase>* pActiveMenuScreen : activeMenuScreens)
	{
		if (pActiveMenuScreen->Get() == pMenuScreen)
		{
			PopMenuScreen(*pActiveMenuScreen);
			return;
		}
	}
}

void MenuSystem::OpenMenu(const OnInputActionDelegateParams& Params)
{
	bMainMenuOpen = true;
	
	if (pInputSystem && pMenuInputContext)
	{
		pInputSystem->PushInputContext(pMenuInputContext);
	}

	PushMenuScreen(pBaseMenuScreen);
}

void MenuSystem::CloseMenu(const OnInputActionDelegateParams& Params)
{
	PopMenuScreen(*activeMenuScreens.back());

	if (bMainMenuOpen && activeMenuScreens.size() == 0)
	{
		// Menu system entirely closed
		bMainMenuOpen = false;
	
		if (pInputSystem && pMenuInputContext)
		{
			pInputSystem->PopInputContext(pMenuInputContext);
		}
	}
}
