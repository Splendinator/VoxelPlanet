#pragma once

#include "UIObjectBase.h"

class VectorPrimitiveShape;

class UIObjectButton : public UIObjectBase
{
protected:
	
	//~ Begin UIObjectBase Interface
	void Init(VectorPrimitiveBase* pRoot) override;
	void Uninit() override;
	void Tick(float deltaTime) override;
	//~ End UIObjectBase Interface

	bool bHoveredOver = false;

	TransientPtr<VectorPrimitiveShape> pButtonPrimitive = nullptr;
};
