#include "pch.h"

#include "ActionDeciderPlayer.h"

#include "ActionHandlerWait.h"
#include "DomWindow/DomWindow.h"
#include "ECS.h"

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

	return nullptr;
}
