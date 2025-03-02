#pragma once

#include "UIObjectContainer.h"

class VectorPrimitiveBase;
class VectorPrimitiveLayer;

// Base UIObject class
// UI Objects wrap around a subset of a UICanvas and provide easy usage
// For example UIObjectProgressBar will wrap around	a single VectorPrimitiveRectangle and provide a simple interface to set the progress (width)
class UIObjectBase : public UIObjectContainer
{
	EDITORBODY()
public:

	// Init and UnInit functions are automatically called by the UIObjectContainer that contains this UIObject
	// pRoot will either be the layer (if multiple primitives are on one layer) or the single primitive in the layer
	void BaseInit(VectorPrimitiveBase* pRoot, const UICanvas* pCanvas);
	void BaseUninit();

	// Optional tick for objects that need anims
	void BaseTick(float deltaTime);

protected:

	// Init objects using UIObjectContainer::AddExistingUIObject()
	virtual void Init(VectorPrimitiveBase* pRoot) {};

	// Opposite of Init()
	virtual void Uninit() {};

	// Called every frame, used for anims etc.
	virtual void Tick(float deltaTime) {};
	
	// Parent canvas of this UI object
	TransientPtr<const UICanvas> pParentCanvas = nullptr;

private:

	const UICanvas* GetCanvas() const final { return pParentCanvas.Get(); }
};
