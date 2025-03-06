#pragma once

#include "Core/GameSystem.h"

#include "ECS/ECSTypes.h"

class DirectoryData;
class ECS;
class HUDObjectBase;

EDITORSTRUCT()
struct HUDObjectSharedInitParams
{
	EDITORBODY()
	
	// Padding percent around the edge of the screen that HUDObjects will not be rendered in
	EDITORPROPERTY()
	float screenEdgePadding = 0.05f;

	ECS* pEcs = nullptr;
};

/** UI
*
* This class is responsible for showing the UI.
* 
*/
EDITORCLASS()
class HUD : public GameSystem
{
	EDITORBODY()
	
public:
	//~ Begin GameSystem Interface
	void Init();
	void UnInit();
	void Tick(float deltaTime);
	//~ End GameSystem Interface

private:

	EDITORPROPERTY()
	DirectoryData* pDirectoryData = nullptr;

	EDITORPROPERTY()
	ECS* pEcs = nullptr;
	
	EDITORPROPERTY()
	std::vector<HUDObjectBase*> pHudObjects;

	// Params shared by all UI objects
	EDITORPROPERTY()
	HUDObjectSharedInitParams hudObjectSharedInitParams;
};