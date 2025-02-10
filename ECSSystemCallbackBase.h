#pragma once

#include "ECSTypes.h"

class ECS;

/** ECSSystemCallbackBase
 *
 * Base class for setting up a callback within an ECS system
 */
class ECSSystemCallbackBase
{
public:
	ECSSystemCallbackBase() = default;
	ECSSystemCallbackBase(const ECSSystemCallbackBase& other) = default;
	ECSSystemCallbackBase(ECSSystemCallbackBase&& other) = default;
	virtual ~ECSSystemCallbackBase() = default;

	ECSSystemCallbackBase& operator=(ECSSystemCallbackBase& other) = default;
	ECSSystemCallbackBase& operator=(ECSSystemCallbackBase&& other) = default;

	void HandleCallbacks(ECS* pEcs, float deltaTime, int frame) { DoCallback(pEcs, deltaTime, frame); }
	void HandleEntityDeletion(ECS* pEcs, EntityId entity) { DoEntityDeletion(pEcs, entity); }

protected:

	virtual void DoCallback(ECS* pEcs, float deltaTime, int frame) = 0;
	virtual void DoEntityDeletion(ECS* pEcs, EntityId entity) = 0;
};
