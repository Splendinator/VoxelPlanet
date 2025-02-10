#pragma once

#include "UI/UICanvas.h"

class DirectoryData;

// A single menu screen 
EDITORCLASS(Abstract, Instanced)
class MenuScreenBase
{
	EDITORBODY()

public:

	void BaseInit();
	void BaseUnInit();
	void BaseTick(float DeltaTime);

protected:
	virtual void Init(UICanvas* pCanvas) {}
	virtual void UnInit() {}
	virtual void Tick(float DeltaTime) {}

	std::unique_ptr<UICanvas> uiCanvas;
	
private:
	
	EDITORPROPERTY()
	DirectoryData* pDirectoryData = nullptr;

	EDITORPROPERTY()
	std::string fileName;
};
