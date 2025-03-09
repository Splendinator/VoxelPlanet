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

	// Push / Pop a screen
	void PushMenuScreen(TInstancedAssetPtr<MenuScreenBase>& pMenuScreenAsset);
	void PopMenuScreen(TInstancedAssetPtr<MenuScreenBase>& pMenuScreenAsset);

	void CloseScreen(MenuScreenBase* pMenuScreen);

	float GetMenuSizeFraction() const { return menuSizeFraction; }
	
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

	// Size of menus relative to the size of the screen (1.0f = fullscreen) 
	EDITORPROPERTY()
	float menuSizeFraction = 1.0f;
	
	void OpenMenu(OnInputActionDelegateParams Params);
	void CloseMenu(OnInputActionDelegateParams Params);

	// Stack of active menus screens, only the screen at the top of the stack will be initialised and ticking.
	std::vector<TInstancedAssetPtr<MenuScreenBase>*> activeMenuScreens;
	
	DelegateClass<MenuSystem, OnInputActionDelegateParams> OpenMenuDelegate;
	DelegateClass<MenuSystem, OnInputActionDelegateParams> CloseMenuDelegate;

	bool bMainMenuOpen = false;
};
