#pragma once

#include "ECSTypes.h"

class ECS;

// Params passed to tick function
struct SystemTickParams
{
	float deltaTime{};
	EntityId entityId{};
	ECS* pEcs{};
	int frame{};
};

// Params passed to entity deletion function
struct SystemEntityDeletionParams
{
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

	// Called before any system callbacks
	virtual void PreTick(SystemTickParams tickParams) {};

protected:
	
	// Should be overridden to register callback to the ECS
	virtual void InitialiseInternal(ECS* pEcs) = 0;
};