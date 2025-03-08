#include "pch.h"

#include "UIObjectBase.h"

void UIObjectBase::BaseInit(VectorPrimitiveLayer* pRoot, const UICanvas* pCanvas)
{
	pParentCanvas = pCanvas;
	
	Init(pRoot);
}

void UIObjectBase::BaseUninit()
{
	UninitAllUIObjects();
	
	Uninit();

	pParentCanvas = nullptr;
}

void UIObjectBase::BaseTick(float deltaTime)
{
	TickAllUIObjects(deltaTime);
	
	Tick(deltaTime);
}

