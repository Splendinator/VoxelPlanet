#pragma once
#include "ECSSystemBase.h"

struct ComponentTransform;

/** ECSSystemCleanUp
*
* This class is used for cleaning up component at the end of the tick. This should be the final system in the array
*/
class ECSSystemCleanUp : public ECSSystemBase
{
public:
	
	void Tick(const ECSSystemTickParams& params, const std::tuple<ComponentTransform*>& components);
	void OnEntityDeleted(const ECSSystemEntityDeletionParams& params, const std::tuple<ComponentTransform*>& components);

protected:
	//~ Begin ECSSystemBase Interface
	void InitialiseInternal(ECS* pEcs) override;
	//~ End ECSSystemBase Interface

};
