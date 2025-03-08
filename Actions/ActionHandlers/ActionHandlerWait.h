#pragma once

#include "ActionHandlerBase.h"

/** ActionHandlerWait
*
* This class is used to wait a turn
*/
EDITORCLASS()
class ActionHandlerWait : public ActionHandlerBase
{
	EDITORBODY()
public:
	
	//~ Begin ActionHandlerBase Interface
	void DoAction(ECS& ecs, EntityId e) override {};
	void Reset() override {};
	//~ End ActionHandlerBase Interface

};
