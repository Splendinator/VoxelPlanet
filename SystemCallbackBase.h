#pragma once

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

	void HandleCallbacks(ECS* pEcs, float deltaTime) { DoCallback(pEcs, deltaTime); }

protected:

	virtual void DoCallback(ECS* pEcs, float deltaTime) = 0;
};
