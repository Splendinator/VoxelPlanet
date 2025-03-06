#pragma once

#include "ActionDeciderBase.h"

struct ComponentFaction;

/** ActionDeciderAI
*
* This class is used for deciding what action an AI will take
*/
class ActionDeciderAI : public ActionDeciderBase
{
public:

	//~ Begin ActionDeciderBase Interface
	ActionHandlerBase* DecideAction(ECS& ecs, EntityId actor) override;
	//~ End ActionDeciderBase Interface

protected:

	bool IsWarringFactions(const ComponentFaction& faction, const ComponentFaction& otherFaction) const;

};
