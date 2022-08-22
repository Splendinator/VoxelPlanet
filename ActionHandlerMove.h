#pragma once

#include "ActionHandlerBase.h"

/** ActionHandlerMove
*
* This class is used for moving an entity
*/
class ActionHandlerMove : public ActionHandlerBase
{
public:

	static ActionHandlerMove& GetSingleton() 
	{ 
		static ActionHandlerMove staticActionHandlerMove;
		return staticActionHandlerMove;
	}
	
	//~ Begin ActionHandlerBase Interface
	void DoAction(ECS& ecs, EntityId e) override;
	void Reset() override { xOffset = yOffset = 0; }
	//~ End ActionHandlerBase Interface

	// How far to move along each axis
	int xOffset = 0;
	int yOffset = 0;


};
