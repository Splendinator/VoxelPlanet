#pragma once
#include "SystemBase.h"

struct ComponentTransform;

/** SystemCleanUp
*
* This class is used for cleaning up component at the end of the tick. This should be the final system in the array
*/
class SystemCleanUp : public SystemBase
{
public:
	
	void Tick(const SystemTickParams& params, const std::tuple<ComponentTransform*>& components);
	void OnEntityDeleted(const SystemEntityDeletionParams& params, const std::tuple<ComponentTransform*>& components);

protected:
	//~ Begin SystemBase Interface
	void InitialiseInternal(ECS* pEcs) override;
	//~ End SystemBase Interface

};
