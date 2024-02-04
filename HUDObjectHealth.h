#pragma once
#pragma once

#include "HUDObjectBase.h"

class UIObjectProgressBar;

/** HUDObjectHealth
*
* This class is used to show the player's health on screen I suppose
*/
class HUDObjectHealth : public HUDObjectBase
{
public:

	HUDObjectHealth(const HUDInitParams& initParams) : HUDObjectBase(initParams) {}

protected:

	//~ Begin HUDObjectBase Interface
	void Init(UICanvas& canvas) override;
	void Tick(ECS& ecs, EntityId player, float deltaTime) override;
	//~ End HUDObjectBase Interface

	TransientPtr<UIObjectProgressBar> pHealthBarProgress;
};