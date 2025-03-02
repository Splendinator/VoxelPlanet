#pragma once

#include "AssetManager/InstancedAssetPtr.h"

#include "Core/GameSystem.h"

#include "Input/InputAction.h"

class MenuScreenBase;
class InputContext;
class InputSystem;

// The menu system keeps a stack of MenuScreens and shows the top one
// i.e -> PauseMenu -> Options -> Video Options
EDITORCLASS()
class MenuSystem : public GameSystem
{
	EDITORBODY()

public:
	//~ Begin GameSystem Interface
	void Init() override;
	void Tick(float deltaTime) override;
	void UnInit() override;
	//~ End GameSystem Interface

	void PushMenuScreen(TInstancedAssetPtr<MenuScreenBase>& pMenuScreenAsset);
	void PopMenuScreen(TInstancedAssetPtr<MenuScreenBase>& pMenuScreenAsset);

	void CloseScreen(MenuScreenBase* pMenuScreen);
	
private:
	// Input system
	EDITORPROPERTY()
	InputSystem* pInputSystem = nullptr;

	// Input context to push while menu is open
	EDITORPROPERTY()
	InputContext* pMenuInputContext = nullptr;

	// Action to open menu
	EDITORPROPERTY()
	InputActionBase* pOpenMenuAction = nullptr;

	// Action to close menu
	EDITORPROPERTY()
	InputActionBase* pCloseMenuAction = nullptr;

	// Base menu screen to open
	EDITORPROPERTY()
	TInstancedAssetPtr<MenuScreenBase> pBaseMenuScreen;

	// Menu screen to open when the game first starts
	EDITORPROPERTY()
	TInstancedAssetPtr<MenuScreenBase> pGameStartMenuScreen;
	
	void OpenMenu(OnInputActionDelegateParams Params);
	void CloseMenu(OnInputActionDelegateParams Params);
	
	std::vector<TInstancedAssetPtr<MenuScreenBase>*> activeMenuScreens;
	
	DelegateClass<MenuSystem, OnInputActionDelegateParams> OpenMenuDelegate;
	DelegateClass<MenuSystem, OnInputActionDelegateParams> CloseMenuDelegate;

	bool bMenuOpen = false;
};
