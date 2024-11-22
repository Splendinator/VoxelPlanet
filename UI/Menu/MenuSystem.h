#pragma once

#include "AssetManager/InstancedAssetPtr.h"

#include "Core/GameSystem.h"

#include "Input/InputAction.h"

class MenuScreenBase;
class InputContext;
class InputSystem;

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
	
	void OpenMenu(OnInputActionDelegateParams Params);
	void CloseMenu(OnInputActionDelegateParams Params);
	
	DelegateClass<MenuSystem, OnInputActionDelegateParams> OpenMenuDelegate;
	DelegateClass<MenuSystem, OnInputActionDelegateParams> CloseMenuDelegate;
};
