#include "pch.h"

#include "UIObjectButton.h"
#include "VectorPrimitiveShape.h"

#include "DomWindow/DomWindow.h"

#include "UI/UICanvas.h"

void UIObjectButton::Init(VectorPrimitiveLayer* pRoot)
{
	pClickablePrimitive = pRoot;	
}

void UIObjectButton::Uninit()
{
	pClickablePrimitive = nullptr;	
}

void UIObjectButton::Tick(float deltaTime)
{
	if (dmwi::isPressed(dmwi::LMB)) // #TODO: Maybe we need a middle man for mouse input? We can probably use input actions 
	{
		const VectorPrimitiveBase* pPrimitiveUnderCursor = pParentCanvas->FindPrimitiveUnderCursor();
		if (pPrimitiveUnderCursor == pClickablePrimitive || pClickablePrimitive->IsChildOfThis(pPrimitiveUnderCursor))
		{
			onClicked.Invoke({this});
		}
	}
}
