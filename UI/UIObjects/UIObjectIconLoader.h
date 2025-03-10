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
 * The size of the new icon is the same as the bounding box of the primitive passed in, so don't pass in an empty layer. 
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
	void Init(VectorPrimitiveLayer* pRoot) override;
	void Uninit() override;
	//~ End UIObjectBase Interface
	
	// This box is used to resize the new vector art to fit
	Box2f iconSize = {};
	
	TransientPtr<VectorPrimitiveLayer> pReplaceThis = nullptr;

	const u32 newIconXPos = 0;
	const u32 newIconYPos = 0;
	const u32 newIconWidth = 0;
	const u32 newIconHeight = 0;
	
};
