#include "pch.h"

#include "MenuScreenMain.h"

#include "DirectoryData.h"

#include "UI/UIObjects/UIObjectButton.h"
#include "UI/UIObjects/UIObjectIconLoader.h"

void MenuScreenMain::Init(UICanvas* pCanvas)
{
	pTestButton = pCanvas->GetUIObject<UIObjectButton>();
	pTestIconLoader = pCanvas->GetUIObject<UIObjectIconLoader>("PlayerIcon");
	pTestIconLoader->LoadUIIcon("C:/Users/Dominic/Desktop/Player.svg");
}

void MenuScreenMain::UnInit()
{
	pTestButton = nullptr;
}

void MenuScreenMain::Tick(float DeltaTime)
{
	
}

