#include "pch.h"

#include "ECSSystemAction.h"

#include "Actions/ActionDeciders/ActionDeciderBase.h"
#include "Actions/ActionHandlers/ActionHandlerBase.h"
#include "ECS/Components.h"
#include "ECS/ECS.h"
#include "ECS/Systems/Callbacks/ECSSystemCallback.h"

void ECSSystemAction::InitialiseInternal(ECS* pEcs)
{
	pEcs->RegisterSystemCallback(std::make_unique<ECSSystemCallback<ECSSystemAction, ComponentAction>>(this));
}

void ECSSystemAction::PreTick(ECSSystemTickParams tickParams)
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

	// Lowest energy of all entities
	if (lowestEnergy > 0)
	{
		tickParams.pEcs->ForEachEntity([&](EntityId e)
			{
				if (tickParams.pEcs->EntityHasComponents<ComponentAction>(e))
				{
					ComponentAction& action = tickParams.pEcs->GetComponent<ComponentAction>(e);
					action.energy -= lowestEnergy;
				}
			});
	}
	
}

void ECSSystemAction::Tick(const ECSSystemTickParams& params, const std::tuple<ComponentAction*>& components)
{
	if (params.frame == frameLastActionTookPlace)
	{
		// Something already took an action this turn
		return; 
	}

	ComponentAction* pAction = std::get<ComponentAction*>(components);

	if (pAction->pActionDecider == nullptr)
	{
		DOMLOG_WARN("No action decider set up on entity", params.entityId);
		return;
	}
	
	if (pAction->energy == 0)
	{
		if (ActionHandlerBase* pActionHandler = pAction->pActionDecider->DecideAction(*params.pEcs, params.entityId))
		{
			pActionHandler->DoAction(*params.pEcs, params.entityId);
			pActionHandler->Reset();

			frameLastActionTookPlace = params.frame;

			pAction->energy = pAction->maxEnergy; // Took turn -- Reset energy to max
		}
	}
}

void ECSSystemAction::OnEntityDeleted(const ECSSystemEntityDeletionParams& params, const std::tuple<ComponentAction*>& components)
{
	
}
