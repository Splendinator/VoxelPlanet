#pragma once

#include "MenuScreenBase.h"

#include "UI/UIObjects/UIObjectButton.h"

class ECS;
class RPGClassData;
class RPGRaceData;
class RPGSystem;
class UIObjectIconLoader;

EDITORSTRUCT()
struct MenuScreenClassSelectEntry
{
	EDITORBODY()

	// Name of the layer the icon inside the class icons file  
	EDITORPROPERTY()
	std::string classIconLayer;

	// Name of the layer of the loader inside the menu screen file
	EDITORPROPERTY()
	std::string loaderLayer;

	// Class 
	EDITORPROPERTY()
	const RPGClassData* pRpgClass = nullptr;

	TransientPtr<UIObjectIconLoader> iconLoader;
	TransientPtr<UIObjectButton> button;
};

EDITORCLASS()
class MenuScreenClassSelect : public MenuScreenBase
{
	EDITORBODY()

protected:
	//~ Begin MenuScreenBase Interface
	void Init(UICanvas* pCanvas) override;
	void UnInit() override;
	//~ End MenuScreenBase Interface

	void OnClicked(UIObjectButtonDelegateParams params);

	EDITORPROPERTY()
	RPGSystem* pRpgSystem = nullptr;

	EDITORPROPERTY()
	ECS* pEcs = nullptr;

	// Race that the player will spawn as
	EDITORPROPERTY()
	const RPGRaceData* pPlayerRace = nullptr;
	
	// Name of class icon file, no .svg
	EDITORPROPERTY()
	std::string classIconFileName;

	// List of all selectable classes in this screen
	EDITORPROPERTY()
	std::vector<MenuScreenClassSelectEntry> classEntries;

	DelegateClass<MenuScreenClassSelect, UIObjectButtonDelegateParams> onClickedDelegate;
	
};
