#pragma once

#include "Core/GameSystem.h"

#include "Input/InputAction.h"

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

	void OpenMenu(OnInputActionDelegateParams Params);
	void CloseMenu(OnInputActionDelegateParams Params);
	
	// #TODO: Needs a editor exposed TAssetPtr<UMenuScreenBase> that allows for dynamic spawning of a *NON SINGLETON* class.
	// #TODO: It will also handle deleting in the destructor so no need for garbage collection system.
	// #TODO: If we want to in the future we can make it a smart pointer with reference count.

	DelegateClass<MenuSystem, OnInputActionDelegateParams> OpenMenuDelegate;
	DelegateClass<MenuSystem, OnInputActionDelegateParams> CloseMenuDelegate;
};
