#include "pch.h"

#include "SystemAction.h"

#include "ActionDeciderBase.h"
#include "ActionHandlerBase.h"
#include "Components.h"
#include "ECS.h"
#include "SystemCallback.h"

void SystemAction::InitialiseInternal(ECS* pEcs)
{
	pEcs->RegisterSystemCallback(std::make_unique<SystemCallback<SystemAction, ComponentAction>>(this));
}

void SystemAction::PreTick(SystemTickParams tickParams)
{
	// Handle energy

	// Find lowest energy
	short lowestEnergy = std::numeric_limits<short>::max();
	tickParams.pEcs->ForEachEntity([&](EntityId e)
		{
			if (tickParams.pEcs->EntityHasComponents<ComponentAction>(e))
			{
				ComponentAction& action = tickParams.pEcs->GetComponent<ComponentAction>(e);
				if (action.energy < lowestEnergy)
				{
					lowestEnergy = action.energy;
				}
			}
		});

	// Lower energy of all entities
	tickParams.pEcs->ForEachEntity([&](EntityId e)
		{
			if (tickParams.pEcs->EntityHasComponents<ComponentAction>(e))
			{
				ComponentAction& action = tickParams.pEcs->GetComponent<ComponentAction>(e);
				action.energy -= lowestEnergy;
			}
		});
	
}

void SystemAction::Tick(const SystemTickParams& params, const std::tuple<ComponentAction*>& components)
{
	if (params.frame == frameLastActionTookPlace)
	{
		// Something already took an action this turn
		return; 
	}

	ComponentAction* pAction = std::get<ComponentAction*>(components);

	if (pAction->energy == 0)
	{
		DOMASSERT(pAction->pActionDecider, "Action decider not set");

		if (ActionHandlerBase* pActionHandler = pAction->pActionDecider->DecideAction(*params.pEcs, params.entityId))
		{
			pActionHandler->DoAction(*params.pEcs, params.entityId);
			pActionHandler->Reset();

			frameLastActionTookPlace = params.frame;

			pAction->energy = pAction->maxEnergy; // Took turn -- Reset energy to max
		}
	}
}

void SystemAction::OnEntityDeleted(const SystemEntityDeletionParams& params, const std::tuple<ComponentAction*>& components)
{
	ComponentAction* pAction = std::get<ComponentAction*>(components);
	
	delete pAction->pActionDecider;
}
