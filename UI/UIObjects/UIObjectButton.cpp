#include "pch.h"

#include "UIObjectButton.h"
#include "VectorPrimitiveShape.h"

#include "UI/UICanvas.h"

void UIObjectButton::Init(VectorPrimitiveBase* pRoot)
{
	UIObjectBase::Init(pRoot);

	pButtonPrimitive = pRoot->FindPrimitiveByLabel<VectorPrimitiveShape>("Button");
}

void UIObjectButton::Uninit()
{
	pButtonPrimitive = nullptr;
	
	UIObjectBase::Uninit();
}

void UIObjectButton::Tick(float deltaTime)
{
	UIObjectBase::Tick(deltaTime);

	if (pParentCanvas->FindPrimitiveUnderCursor() == pButtonPrimitive.Get())
	{
		Vec3<u8> colour = pButtonPrimitive->GetColour();
		pButtonPrimitive->SetColour(colour.x + 1, colour.y + 1, colour.z + 1);
	}
}
