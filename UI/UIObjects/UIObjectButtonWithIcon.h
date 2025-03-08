#pragma once

#include "UIObjectBase.h"

class UIObjectButton;
class UIObjectIconLoader;

// Combines UIObjectIconLoader and UIObjectButton to create a clickable icon
class UIObjectButtonWithIcon : public UIObjectBase
{

public:
	void LoadUIIcon(const std::string& file, const std::string& icon);
	UIObjectButton& GetButton() {return *pButton; }
	
protected:
	//~ Begin UIObjectBase Interface
	void Init(VectorPrimitiveLayer* pRoot) override;
	void Uninit() override;
	//~ End UIObjectBase Interface

	TransientPtr<UIObjectButton> pButton;
	TransientPtr<UIObjectIconLoader> pIconLoader;
};
