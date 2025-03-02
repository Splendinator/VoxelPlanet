#pragma once

#include "UIObjectBase.h"

class VectorPrimitiveRectangle;
class VectorPrimitiveLayer;
class VectorArt;

/**
 * This UIObject takes in a VectorPrimitiveLayer and replaces the primitives on that layer with primitives from another external layer
 * The code would look something like this:
 * UIObjectIconLoader* pIconLoader = UICanvas->AddExistingUIObject<UIObjectIconLoader>("SpellIcon")
 * pIconLoader->LoadIcon("SpellIcons.svg", "Fireball")
 *
 * The layer you pass in should contain a single rectangle that will be used for sizes #TODO: We can add a generic bounding box if we want the freedom to use circles too etc.
 *
 * This would replace the contents of layer "SpellIcon" in the base file with the contents of the layer "Fireball" in the SpellIcons.svg
 */
class UIObjectIconLoader : public UIObjectBase
{
public:
	
	// Load a specific file and optionally a specific primitive from within that file
	void LoadUIIcon(const std::string& file, const std::string& primitiveName = "");
	
protected:
	//~ Begin UIObjectBase Interface
	void Init(VectorPrimitiveBase* pRoot) override;
	void Uninit() override;
	//~ End UIObjectBase Interface

	std::unique_ptr<VectorArt> pLoadedVectorArt = nullptr;

	// This rectangle is used to resize the new vector art to fit
	// #TODO: We can add a generic bounding box if we want the freedom to use circles too etc.
	TransientPtr<VectorPrimitiveRectangle> pRectangleForSize = nullptr;
	
	TransientPtr<VectorPrimitiveLayer> pReplaceWithThis = nullptr; 
	TransientPtr<VectorPrimitiveLayer> pReplaceThis = nullptr;

	const u32 newIconXPos = 0;
	const u32 newIconYPos = 0;
	const u32 newIconWidth = 0;
	const u32 newIconHeight = 0;
	
};
