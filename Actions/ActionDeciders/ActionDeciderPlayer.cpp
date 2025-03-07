#include "pch.h"

#include "ActionDeciderPlayer.h"

#include "Actions/ActionHandlers/ActionHandlerWait.h"
#include "DomWindow/DomWindow.h"
#include "ECS/ECS.h"
#include "RPGSystems/Skills/RPGSkillData.h"
#include "RPGSystems/Skills/RPGSkillSystem.h"

ActionHandlerBase* ActionDeciderPlayer::DecideAction(ECS& ecs, EntityId e)
{
	// Movement
	if (dmwi::isPressed(dmwi::Button::NUMPAD7)) { return TryMoveOrAttack(ecs, e, -1, -1); }
	if (dmwi::isPressed(dmwi::Button::NUMPAD8)) { return TryMoveOrAttack(ecs, e,  0, -1); }
	if (dmwi::isPressed(dmwi::Button::NUMPAD9)) { return TryMoveOrAttack(ecs, e,  1, -1); }
	if (dmwi::isPressed(dmwi::Button::NUMPAD4)) { return TryMoveOrAttack(ecs, e, -1,  0); }
	if (dmwi::isPressed(dmwi::Button::NUMPAD6)) { return TryMoveOrAttack(ecs, e,  1,  0); }
	if (dmwi::isPressed(dmwi::Button::NUMPAD1)) { return TryMoveOrAttack(ecs, e, -1,  1); }
	if (dmwi::isPressed(dmwi::Button::NUMPAD2)) { return TryMoveOrAttack(ecs, e,  0,  1); }
	if (dmwi::isPressed(dmwi::Button::NUMPAD3)) { return TryMoveOrAttack(ecs, e,  1,  1); }

	// Wait
	if (dmwi::isPressed(dmwi::Button::NUMPAD5)) { return &ActionHandlerWait::GetSingleton(); }

	// #TEMP: The whole action decider system needs re-doing with editor exposed shit and we need to re-do this
	// Cast Spell 
	if (pSkillSystem)
	{
		if (dmwi::isPressed(dmwi::Button::NUM1))
		{
			bAimingSkill = true;
			pSkillSystem->PlayerStartAimingSkill(Game::GetAssetManager().LoadObjectFromAssetName<RPGSkillData>("RPGSkillData_Temp"));
		}
		if (dmwi::isPressed(dmwi::Button::RMB))
		{
			bAimingSkill = false;
			pSkillSystem->StopAimingSkill();
		}
		if (dmwi::isPressed(dmwi::Button::LMB))
		{
			if (bAimingSkill)
			{
				const bool bFired = pSkillSystem->TryFirePlayerAimedSkill();
				if (bFired)
				{
					return &ActionHandlerWait::GetSingleton();
				}
			}
		}
	}

	return nullptr;
}
