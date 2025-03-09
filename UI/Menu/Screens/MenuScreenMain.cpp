#include "pch.h"

#include "MenuScreenMain.h"

#include "UI/Menu/MenuSystem.h"
#include "UI/UIObjects/UIObjectButton.h"

void MenuScreenMain::Init(UICanvas* pCanvas)
{
	onButtonPressedDelegate.Bind(this, &MenuScreenMain::OnButtonPressed);
	
	for (MainMenuButtonData& buttonData : buttonDatas)
	{
		buttonData.pButton = pCanvas->AddExistingUIObject<UIObjectButton>(buttonData.buttonLayerName);
		if (buttonData.pButton)
		{
			buttonData.pButton->onClicked.Add(onButtonPressedDelegate);
		}
	}
}

void MenuScreenMain::UnInit()
{
	for (MainMenuButtonData& buttonData : buttonDatas)
	{
		if (buttonData.pButton)
		{
			buttonData.pButton->onClicked.Remove(onButtonPressedDelegate);
		}
	}
}

void MenuScreenMain::Tick(float DeltaTime)
{
	
}

void MenuScreenMain::OnButtonPressed(UIObjectButtonDelegateParams params)
{
	for (MainMenuButtonData& buttonData : buttonDatas)
	{
		if (buttonData.pButton.Get() == params.pButton)
		{
			pMenuSystem->PushMenuScreen(buttonData.pMenuScreenToOpen);
		}
	}
}
