#include "pch.h"

#include "MenuScreenBase.h"

#include "DirectoryData.h"
#include "DomWindow/DomWindow.h"

void MenuScreenBase::BaseInit()
{
	UICanvasInitParams initParams;
	initParams.sizeX = (float)dmwi::getWindowWidth();//* 3.5f;
	initParams.sizeY = (float)dmwi::getWindowHeight();// * 3.5f;
	initParams.filePath = pDirectoryData->menus + fileName + ".svg";
	initParams.renderPriority = RenderPriority::menu;
	initParams.type = dmgf::ERenderObjectType::UI;

	uiCanvas = std::make_unique<UICanvas>(initParams);

	Init(uiCanvas.get());
}

void MenuScreenBase::BaseTick(float DeltaTime)
{
	uiCanvas->Tick(DeltaTime);
	
	Tick(DeltaTime);
}

void MenuScreenBase::BaseUnInit()
{
	UnInit();
	
	uiCanvas->UnInit();
	uiCanvas.reset();	
}



