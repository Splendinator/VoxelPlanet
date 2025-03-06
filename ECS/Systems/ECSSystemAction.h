#pragma once

#include "ECSSystemBase.h"

struct ComponentAction;

/** ECSSystemAction
*
* This system is used to allow entities with an action component to take actions
*/
EDITORCLASS()
class ECSSystemAction : public ECSSystemBase
{
	EDITORBODY()
public:

	//~ Begin ECSSystemBase Interface
	void PreTick(ECSSystemTickParams tickParams) override;
	//~ End ECSSystemBase Interface

	void Tick(const ECSSystemTickParams& params, const std::tuple<ComponentAction*>& components);
	void OnEntityDeleted(const ECSSystemEntityDeletionParams& params, const std::tuple<ComponentAction*>& components);;

protected:
	
	//~ Begin ECSSystemBase Interface
	void InitialiseInternal(ECS* pEcs) override;
	//~ End ECSSystemBase Interface

	int frameLastActionTookPlace = 0; // Start this at 0 instead of -1 so that we skip the first turn and let other systems boot up correctly
};
