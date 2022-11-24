#pragma once

#include "UIObjectContainer.h"

class VectorPrimitiveBase;

// Base UIObject class
// UI Objects wrap around a subset of a UICanvas and provide easy usage
// For example UIObjectProgressBar will wrap around	a single VectorPrimitiveRectangle and provide a simple interface to set the progress
class UIObjectBase : public UIObjectContainer
{
public:

	// Init and Uninit functions are automatically called by the UIObjectContainer that contains this UIObject
	void BaseInit(VectorPrimitiveBase* pRoot);
	void BaseUninit();

	// Optional tick for objects that need anims
	void BaseTick(float deltaTime);

protected:

	// Init objects using UIObjectContainer::CreateUIObject()
	virtual void Init(VectorPrimitiveBase* pRoot) {};

	// Opposite of Init()
	virtual void Uninit() {};

	// Called every frame, used for anims etc.
	virtual void Tick(float deltaTime) {};
};
