#include "pch.h"

#include "ECSSystemAction.h"

#include <algorithm>

#include "AI/Actions/ActionDeciders/ActionDeciderBase.h"
#include "AI/Actions/ActionHandlers/ActionHandlerBase.h"
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
	u16 lowestEnergy = std::numeric_limits<short>::max();
	tickParams.pEcs->ForEachEntity([&](EntityId e)
		{
			if (tickParams.pEcs->EntityHasComponents<ComponentAction>(e))
			{
				ComponentAction& action = tickParams.pEcs->GetComponent<ComponentAction>(e);
				lowestEnergy = std::min(action.energy, lowestEnergy);
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

	bSomethingActedThisTurn = false;
}

void ECSSystemAction::Tick(const ECSSystemTickParams& params, const std::tuple<ComponentAction*>& components)
{
	if (bSomethingActedThisTurn || params.frame == 0)
	{
		// Skip first frame to give other systems a chance to boot up correctly
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

			pAction->energy = pAction->maxEnergy; // Took turn -- Reset energy to max

			bSomethingActedThisTurn = !pActionHandler->bCanThisHappenOnTheSameFrameAsOtherActions;
		}
	}
}

void ECSSystemAction::OnEntityDeleted(const ECSSystemEntityDeletionParams& params, const std::tuple<ComponentAction*>& components)
{
	
}
