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

void SystemAction::Tick(const SystemTickParams& params, const std::tuple<ComponentAction*>& components)
{
	ComponentAction* pAction = std::get<ComponentAction*>(components);

	DOMASSERT(pAction->pActionDecider, "Action decider not set");

	if (ActionHandlerBase* pActionHandler = pAction->pActionDecider->DecideAction(*params.pEcs, params.entityId))
	{
		pActionHandler->DoAction(*params.pEcs, params.entityId);
		pActionHandler->Reset();
	}
}
