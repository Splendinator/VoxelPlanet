#include "pch.h"

#include "UIObjectBase.h"

void UIObjectBase::BaseInit(VectorPrimitiveBase* pRoot)
{
	Init(pRoot);
}

void UIObjectBase::BaseUninit()
{
	UninitAllUIObjects();
	
	Uninit();
}

void UIObjectBase::BaseTick(float deltaTime)
{
	TickAllUIObjects(deltaTime);
	
	Tick(deltaTime);
}
