#include "pch.h"

#include "MenuSystem.h"

#include "Input/InputAction.h"
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
	
}

void MenuSystem::UnInit()
{
	if (pOpenMenuAction)
	{
		pOpenMenuAction->OnActionActivated.Remove(OpenMenuDelegate);
	}
	if (pCloseMenuAction)
	{
		pCloseMenuAction->OnActionActivated.Remove(CloseMenuDelegate);
	}
}

void MenuSystem::OpenMenu(OnInputActionDelegateParams Params)
{
	if (pInputSystem && pMenuInputContext)
	{
		pInputSystem->PushInputContext(pMenuInputContext);
	}
}

void MenuSystem::CloseMenu(OnInputActionDelegateParams Params)
{
	if (pInputSystem && pMenuInputContext)
	{
		pInputSystem->PopInputContext(pMenuInputContext);
	}
}