#pragma once

#include "UI/UICanvas.h"

class MenuSystem;
class DirectoryData;

// A single menu screen 
EDITORCLASS(Abstract, Instanced)
class MenuScreenBase
{
	EDITORBODY()

public:

	// Right now the screen is only initialised while it's top of the MenuSystem stack.
	// #NOTE: I'm not sure if this is what we want, screens on the bottom should stay active probably so they can keep state?
	void BaseInit(MenuSystem* pInMenuSystem);
	void BaseUnInit();
	void BaseTick(float DeltaTime);

protected:
	virtual void Init(UICanvas* pCanvas) {}
	virtual void UnInit() {}
	virtual void Tick(float DeltaTime) {}

	// Tell the menu system to close self
	void RequestCloseSelf();
	
	std::unique_ptr<UICanvas> uiCanvas;

	EDITORPROPERTY()
	DirectoryData* pDirectoryData = nullptr;

	TransientPtr<MenuSystem> pMenuSystem = nullptr;
	
private:

	EDITORPROPERTY()
	std::string fileName;

	bool bWantsToClose = false;
};
