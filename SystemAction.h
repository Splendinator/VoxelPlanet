#pragma once

#include "SystemBase.h"

struct ComponentAction;

/** SystemAction
*
* This system is used to allow entities with an action component to take actions
*/
class SystemAction : public SystemBase
{
public:

	//~ Begin SystemBase Interface
	void PreTick(SystemTickParams tickParams) override;
	//~ End SystemBase Interface

	void Tick(const SystemTickParams& params, const std::tuple<ComponentAction*>& components);
	void OnEntityDeleted(const SystemEntityDeletionParams& params, const std::tuple<ComponentAction*>& components);;

protected:
	
	//~ Begin SystemBase Interface
	void InitialiseInternal(ECS* pEcs) override;
	//~ End SystemBase Interface

	int frameLastActionTookPlace = 0; // Start this at 0 instead of -1 so that we skip the first turn and let other systems boot up correctly
};
