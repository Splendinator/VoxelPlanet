#include "pch.h"

#include "UIObjectButtonWithIcon.h"

#include "UIObjectButton.h"
#include "UIObjectIconLoader.h"

void UIObjectButtonWithIcon::Init(VectorPrimitiveLayer* pRoot)
{
	pButton = AddUIObject<UIObjectButton>(pRoot);
	pIconLoader = AddUIObject<UIObjectIconLoader>(pRoot);
}

void UIObjectButtonWithIcon::Uninit()
{
	pButton = nullptr;
	pIconLoader = nullptr;
}

void UIObjectButtonWithIcon::LoadUIIcon(const std::string& file, const std::string& icon)
{
	pIconLoader->LoadUIIcon(file, icon);
	
}
