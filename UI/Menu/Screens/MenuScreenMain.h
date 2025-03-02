#pragma once

#include "MenuScreenBase.h"

class UIObjectIconLoader;
class UIObjectButton;

// This is the main menu that opens when you hit ESC
EDITORCLASS()
class MenuScreenMain : public MenuScreenBase
{
	EDITORBODY()

public:
	//~ Begin MenuScreenBase Interface
	void Init(UICanvas* pCanvas) override;
	void UnInit() override;
	void Tick(float DeltaTime) override;
	//~ End MenuScreenBase Interface
};
