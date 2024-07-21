#pragma once
#pragma once

#include "HUDObjectBase.h"

class UIObjectProgressBar;

/** HUDObjectHealth
*
* This class is used to show the player's health on screen I suppose
*/
EDITORCLASS()
class HUDObjectHealth : public HUDObjectBase
{
	EDITORBODY()

protected:

	//~ Begin HUDObjectBase Interface
	void Init(UICanvas& canvas) override;
	void Tick(ECS& ecs, EntityId player, float deltaTime) override;
	//~ End HUDObjectBase Interface

	TransientPtr<UIObjectProgressBar> pHealthBarProgress;
};