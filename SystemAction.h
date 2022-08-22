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

	void Tick(const SystemTickParams& params, const std::tuple<ComponentAction*>& components);

protected:
	
	//~ Begin SystemBase Interface
	void InitialiseInternal(ECS* pEcs) override;
	//~ End SystemBase Interface
};
