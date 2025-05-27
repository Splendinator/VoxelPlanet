#include "pch.h"

#include "UIObjectButton.h"
#include "Graphics/VectorArt/VectorPrimitiveShape.h"

#include "DomWindow/DomWindow.h"

#include "UI/UICanvas.h"

void UIObjectButton::Init(VectorPrimitiveLayer* pRoot)
{
	pClickableLayer = pRoot;	
}

void UIObjectButton::Uninit()
{
	pClickableLayer = nullptr;	
}

void UIObjectButton::Tick(float deltaTime)
{
	if (dmwi::isPressed(dmwi::LMB)) // #TODO: Maybe we need a middle man for mouse input? We can probably use input actions 
	{
		const VectorPrimitiveBase* pPrimitiveUnderCursor = pParentCanvas->FindPrimitiveUnderCursor();
		if (pPrimitiveUnderCursor == pClickableLayer || pClickableLayer->IsChildOfThis(pPrimitiveUnderCursor))
		{
			onClicked.Invoke({this});
		}
	}
}
