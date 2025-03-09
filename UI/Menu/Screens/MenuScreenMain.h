#pragma once

#include "MenuScreenBase.h"

struct UIObjectButtonDelegateParams;
class UIObjectIconLoader;
class UIObjectButton;

EDITORSTRUCT()
struct MainMenuButtonData
{
	EDITORBODY()

	EDITORPROPERTY()
	std::string buttonLayerName;
	
	EDITORPROPERTY()
	TInstancedAssetPtr<MenuScreenBase> pMenuScreenToOpen;

	TransientPtr<UIObjectButton> pButton = nullptr;
};

// This is the main menu that opens when you hit ESC
EDITORCLASS()
class MenuScreenMain : public MenuScreenBase
{
	EDITORBODY()

protected:
	//~ Begin MenuScreenBase Interface
	void Init(UICanvas* pCanvas) override;
	void UnInit() override;
	void Tick(float DeltaTime) override;
	//~ End MenuScreenBase Interface

	EDITORPROPERTY()
	std::vector<MainMenuButtonData> buttonDatas;


private:

	DelegateClass<MenuScreenMain, UIObjectButtonDelegateParams> onButtonPressedDelegate;
	
	void OnButtonPressed(UIObjectButtonDelegateParams params);
};
