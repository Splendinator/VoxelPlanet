#pragma once

#include "UIObjectBase.h"

class UIObjectButton;
class VectorPrimitiveShape;

struct UIObjectButtonDelegateParams
{
	UIObjectButton* pButton = nullptr;
};

class UIObjectButton : public UIObjectBase
{

public:
	DelegateList<UIObjectButtonDelegateParams> onClicked; // Fires when button clicked
	
protected:
	
	//~ Begin UIObjectBase Interface
	void Init(VectorPrimitiveLayer* pRoot) override;
	void Uninit() override;
	void Tick(float deltaTime) override;
	//~ End UIObjectBase Interface

	bool bHoveredOver = false;

	TransientPtr<VectorPrimitiveBase> pClickablePrimitive;
};
