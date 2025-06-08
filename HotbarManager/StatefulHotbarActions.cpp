#include "pch.h"

#include "StatefulHotbarActions.h"

#include "AI/Actions/ActionHandlers/ActionHandlerSkill.h"

#include "Input/InputAction.h"
#include "Input/InputSystem.h"

#include "RPGSystems/Skills/RPGSkillSystem.h"

// StatefulHotbarActionCastSkill
void StatefulHotbarActionCastSkill::ToggleOnStatefulAction()
{
	if (pSkill && pSkillSystem)
	{
		pSkillSystem->PlayerStartAimingSkill(pSkill);

		if (pInputSystem && pAimingSkillInputContext)
		{
			pInputSystem->PushInputContext(pAimingSkillInputContext);
		}

		if (pConfirmSkillAction)
		{
			onConfirmSkillDelegate.Bind(this, &StatefulHotbarActionCastSkill::OnConfirmSkill);
			pConfirmSkillAction->onActionActivatedDelegates.Add(onConfirmSkillDelegate);
		}

		if (pCancelSkillAction)
		{
			onCancelSkillDelegate.Bind(this, &StatefulHotbarActionCastSkill::OnCancelSkill);
			pCancelSkillAction->onActionActivatedDelegates.Add(onCancelSkillDelegate);
		}
	}
}

void StatefulHotbarActionCastSkill::ToggleOffStatefulAction()
{
	if (pSkill && pSkillSystem)
	{
		pSkillSystem->StopAimingSkill();

		if (pInputSystem && pAimingSkillInputContext)
		{
			pInputSystem->PopInputContext(pAimingSkillInputContext);
		}

		if (pConfirmSkillAction)
		{
			pConfirmSkillAction->onActionActivatedDelegates.Remove(onConfirmSkillDelegate);
		}

		if (pCancelSkillAction)
		{
			pCancelSkillAction->onActionActivatedDelegates.Remove(onCancelSkillDelegate);
		}
	}
}

void StatefulHotbarActionCastSkill::OnConfirmSkill(const OnInputActionDelegateParams& params)
{
	pActionHandlerSkill->Setup(pSkill, pSkillSystem->GetPlayerAimLocation());
	
	onPlayerActionDecided.Invoke(pActionHandlerSkill); // Calls ToggleOffStatefulAction()
}

void StatefulHotbarActionCastSkill::OnCancelSkill(const OnInputActionDelegateParams& params)
{
	onRequestCancelStatefulAction.Invoke(); // Calls ToggleOffStatefulAction()
}
