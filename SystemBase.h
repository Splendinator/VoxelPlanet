#pragma once

#include "ECSTypes.h"

class ECS;

struct SystemTickParams
{
	float deltaTime{};
	EntityId entityId{};
	ECS* pEcs{};
};

/** SystemBase
 *  
 * Base system class to be used with ECS
 */
class SystemBase
{
public:
	SystemBase() = default;
	SystemBase(const SystemBase& other) = default;
	SystemBase(SystemBase&& other) = default;
	virtual ~SystemBase() = default;

	SystemBase& operator=(SystemBase& other) = default;
	SystemBase& operator=(SystemBase&& other) = default;


	void Initialise(ECS* pEcs) { InitialiseInternal(pEcs); };

protected:
	
	// Should be overridden to register callback to the ECS
	virtual void InitialiseInternal(ECS* pEcs) = 0;
};