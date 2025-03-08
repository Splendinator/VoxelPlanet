#pragma once

#include "ActionHandlerBase.h"

class RPGSystem;

/** ActionHandlerAttack
*
* This class is used for doing a melee attack 
*/
EDITORCLASS()
class ActionHandlerAttack : public ActionHandlerBase
{
	EDITORBODY()
public:
	
	//~ Begin ActionHandlerBase Interface
	void DoAction(ECS& ecs, EntityId e) override;
	void Reset() override;
	//~ End ActionHandlerBase Interface

	EDITORPROPERTY()
	RPGSystem* pRpgSystem = nullptr;

	EntityId target = INVALID_ENTITY_ID; // Target
};
