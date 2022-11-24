#pragma once

#include "ECSTypes.h"

class ECS;

/** SystemCallbackBase
 *
 * Base class for setting up a callback within an ECS system
 */
class SystemCallbackBase
{
public:
	SystemCallbackBase() = default;
	SystemCallbackBase(const SystemCallbackBase& other) = default;
	SystemCallbackBase(SystemCallbackBase&& other) = default;
	virtual ~SystemCallbackBase() = default;

	SystemCallbackBase& operator=(SystemCallbackBase& other) = default;
	SystemCallbackBase& operator=(SystemCallbackBase&& other) = default;

	void HandleCallbacks(ECS* pEcs, float deltaTime, int frame) { DoCallback(pEcs, deltaTime, frame); }
	void HandleEntityDeletion(ECS* pEcs, EntityId entity) { DoEntityDeletion(pEcs, entity); }

protected:

	virtual void DoCallback(ECS* pEcs, float deltaTime, int frame) = 0;
	virtual void DoEntityDeletion(ECS* pEcs, EntityId entity) = 0;
};
