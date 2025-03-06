#pragma once

#include "ActionHandlerBase.h"

/** ActionHandlerWait
*
* This class is used to wait a turn
*/
class ActionHandlerWait : public ActionHandlerBase
{
public:

	static ActionHandlerWait& GetSingleton()
	{ 
		static ActionHandlerWait staticActionHandlerWait;
		return staticActionHandlerWait;
	}

	//~ Begin ActionHandlerBase Interface
	void DoAction(ECS& ecs, EntityId e) override {};
	void Reset() override {};
	//~ End ActionHandlerBase Interface

};
