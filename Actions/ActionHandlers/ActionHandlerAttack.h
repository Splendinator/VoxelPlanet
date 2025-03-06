#pragma once

#include "ActionHandlerBase.h"

/** ActionHandlerAttack
*
* This class is used for doing a melee attack 
*/
class ActionHandlerAttack : public ActionHandlerBase
{
public:

	static ActionHandlerAttack& GetSingleton() 
	{ 
		static ActionHandlerAttack staticActionHandlerAttack;
		return staticActionHandlerAttack;
	}

	//~ Begin ActionHandlerBase Interface
	void DoAction(ECS& ecs, EntityId e) override;
	void Reset() override;
	//~ End ActionHandlerBase Interface


	EntityId enemy = INVALID_ENTITY_ID; // Enemy to attack
	int damage = 0; // Damage to deal

};
