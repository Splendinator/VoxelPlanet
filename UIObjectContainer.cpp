#include "pch.h"

#include "UIObjectContainer.h"

#include "UIObjectBase.h"

void UIObjectContainer::TickAllUIObjects(float deltaTime)
{
	for(UIObjectBase* pChild : pChildren)
	{
		pChild->BaseTick(deltaTime);
	}
}

void UIObjectContainer::UninitAllUIObjects()
{
	for (UIObjectBase* pChild : pChildren)
	{
		pChild->BaseUninit();
		delete pChild;
	}

	pChildren.clear();
}
