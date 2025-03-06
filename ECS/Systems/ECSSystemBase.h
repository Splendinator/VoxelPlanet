#pragma once

#include "ECS/ECSTypes.h"

class ECS;

// Params passed to tick function
struct ECSSystemTickParams
{
	float deltaTime{};
	EntityId entityId{};
	ECS* pEcs{};
	int frame{};
};

// Params passed to entity deletion function
struct ECSSystemEntityDeletionParams
{
	EntityId entityId{};
	ECS* pEcs{};
};

/** ECSSystemBase
 *  
 * Base system class to be used with ECS
 */
EDITORCLASS(Abstract, Singleton)
class ECSSystemBase
{
	EDITORBODY()
public:
	ECSSystemBase() = default;
	ECSSystemBase(const ECSSystemBase& other) = default;
	ECSSystemBase(ECSSystemBase&& other) = default;
	virtual ~ECSSystemBase() = default;

	ECSSystemBase& operator=(ECSSystemBase& other) = default;
	ECSSystemBase& operator=(ECSSystemBase&& other) = default;


	void Initialise(ECS* pEcs) { InitialiseInternal(pEcs); };

	// Called before any system callbacks
	virtual void PreTick(ECSSystemTickParams tickParams) {};

protected:
	
	// Should be overridden to register callback to the ECS
	virtual void InitialiseInternal(ECS* pEcs) { PUREVIRTUAL() };
};