#include "pch.h"

#include "MenuScreenMain.h"

#include "UI/UIObjects/UIObjectButton.h"

void MenuScreenMain::Init(UICanvas* pCanvas)
{
	pTestButton = pCanvas->GetUIObject<UIObjectButton>();
}

void MenuScreenMain::UnInit()
{
	pTestButton = nullptr;
}

void MenuScreenMain::Tick(float DeltaTime)
{
	
}

