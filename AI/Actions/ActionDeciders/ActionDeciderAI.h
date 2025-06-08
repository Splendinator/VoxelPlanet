#pragma once

#include "ActionDeciderBase.h"

class ActionHandlerWait;
struct ComponentFaction;

/** ActionDeciderAI
*
* This class is used for deciding what action an AI will take
*/
EDITORCLASS()
class ActionDeciderAI : public ActionDeciderBase
{
	EDITORBODY()
public:

	//~ Begin ActionDeciderBase Interface
	ActionHandlerBase* DecideAction(ECS& ecs, EntityId actor) override;
	//~ End ActionDeciderBase Interface

protected:

	bool IsWarringFactions(const ComponentFaction& faction, const ComponentFaction& otherFaction) const;

	
	EDITORPROPERTY()
	ActionHandlerWait* pWaitAction = nullptr;

	EDITORPROPERTY()
	int aggroRange;
};
