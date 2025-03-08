#pragma once

#include "HUDObjectBase.h"

class RPGSystem;
class UIObjectProgressBar;

EDITORCLASS()
class HUDObjectXP : public HUDObjectBase
{
	EDITORBODY()

protected:
	
	//~ Begin HUDObjectBase Interface
	void Init(UICanvas& canvas) override;
	void Uninit() override;
	void Tick(ECS& ecs, EntityId player, float deltaTime) override;
	//~ End HUDObjectBase Interface

	TransientPtr<UIObjectProgressBar> pXpProgressBar = nullptr;

	EDITORPROPERTY()
	const RPGSystem* pRpgSystem = nullptr;
};
