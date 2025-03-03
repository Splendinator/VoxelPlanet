#include "pch.h"

#include "MenuScreenBase.h"

#include "DirectoryData.h"
#include "DomWindow/DomWindow.h"

#include "UI/Menu/MenuSystem.h"

void MenuScreenBase::BaseInit(MenuSystem* pInMenuSystem)
{
	DOMASSERT(pInMenuSystem);
	pMenuSystem = pInMenuSystem;
	
	UICanvasInitParams initParams;
	initParams.sizeX = (float)dmwi::getWindowWidth();
	initParams.sizeY = (float)dmwi::getWindowHeight();
	initParams.filePath = DirectoryData::ConcatenateFilePathChecked(pDirectoryData->menus, fileName, ".svg");
	initParams.renderPriority = RenderPriority::menu;
	initParams.type = dmgf::ERenderObjectType::UI;

	uiCanvas = std::make_unique<UICanvas>(initParams);

	Init(uiCanvas.get());
}

void MenuScreenBase::BaseTick(float DeltaTime)
{
	uiCanvas->Tick(DeltaTime);
	
	Tick(DeltaTime);

	if (bWantsToClose && pMenuSystem)
	{
		pMenuSystem->CloseScreen(this);
	}
}

void MenuScreenBase::BaseUnInit()
{
	UnInit();
	
	uiCanvas->UnInit();
	uiCanvas.reset();

	pMenuSystem = nullptr;
}

void MenuScreenBase::RequestCloseSelf()
{
	// The game will crash if we just close half way through a tick etc. so we need to defer
	bWantsToClose = true;
}