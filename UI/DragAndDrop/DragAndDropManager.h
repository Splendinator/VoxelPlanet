#pragma once

#include "Core/GameSystem.h"

#include "DragAndDropShared.h"

class IDragAndDroppable;
class UIObjectIconLoader;
class DirectoryData;
class IDragAndDropArea;
class UICanvas;
struct DragAndDropAreaRegisterParams;

EDITORCLASS()
class DragAndDropManager : public GameSystem
{
	EDITORBODY()
public:
	//~ Begin GameSystem Interface
	void Init() override;
	void Tick(float deltaTime) override;
	void UnInit() override;
#if DOMIMGUI
	void DrawImGui(float deltaTime) override;
#endif
	
	//~ End GameSystem Interface
	
	void RegisterArea(IDragAndDropArea& area, DragAndDropAreaRegisterParams& params);
	void UnregisterArea(IDragAndDropArea& area);

protected:
	
	IDragAndDropArea* GetAreaUnderMouse() const;
	
	// Size of floating icon
	EDITORPROPERTY()
	float floatingIconSize = 0.0f;

	// Name of .sgv file containing floating icon loader
	EDITORPROPERTY()
	std::string floatingIconFileName;

	// Name of the layer to load the drag and drop icon in to
	EDITORPROPERTY()
	std::string iconLoaderLayerName;

	EDITORPROPERTY()
	float floatingIconXOffset = 0.0f;

	EDITORPROPERTY()
	float floatingIconYOffset = 0.0f;
	
	EDITORPROPERTY()
	DirectoryData* pDirectoryData = nullptr;

	// Currently held drag and droppable, this is nullptr while not holding anything 
	TransientPtr<IDragAndDroppable> pCurrentlyHeldDragAndDroppable = nullptr;

	std::unique_ptr<UICanvas> floatingIconCanvas;
	TransientPtr<UIObjectIconLoader> floatingIconLoader;

	// Hashed by priority
	std::vector<IDragAndDropArea*> registeredAreas[(int)EDragAndDropAreaPriority::COUNT];
};
