#pragma once

#include "StatefulHotbarActions.h"

class ActionDeciderBase;
class DirectoryData;

struct OnHotbarItemUsedParams
{
	// Set this if you want the player to do an immediate fire-and-forget action (e.g. use a healing potion)
	ActionHandlerBase* pOutPlayerActionHandler = nullptr;

	// Set this if you need a more complex stateful action (e.g. start aiming a spell that you will later need to aim and confirm before firing)
	std::unique_ptr<StatefulHotbarActionBase> pOutStatefulPlayerAction = nullptr; 	
};

// Anything that wants to be assigned to a hotbar slot should implement this interface.
EDITORCLASS(Abstract)
class IHotbarItem
{
	EDITORBODY()
public:
	
	// Override this to do something when used on the hotbar, optionally returning an action for the player entity to take
	virtual void OnHotbarItemUsed(OnHotbarItemUsedParams& params) const { PUREVIRTUAL() }
	
	
};
