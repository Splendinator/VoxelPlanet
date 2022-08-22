#include "pch.h"

#include "ActionDeciderPlayer.h"

#include "ActionHandlerMove.h"
#include "DomWindow/DomWindow.h"

ActionHandlerBase* ActionDeciderPlayer::DecideAction(ECS& ecs, EntityId e)
{
	// Movement
	if (dmwi::isPressed(dmwi::W))
	{
		ActionHandlerMove& actionHandlerMove = ActionHandlerMove::GetSingleton();
		actionHandlerMove.yOffset = -1;
		return &actionHandlerMove;
	}
	if (dmwi::isPressed(dmwi::A))
	{
		ActionHandlerMove& actionHandlerMove = ActionHandlerMove::GetSingleton();
		actionHandlerMove.xOffset = -1;
		return &actionHandlerMove;
	}
	if (dmwi::isPressed(dmwi::S))
	{
		ActionHandlerMove& actionHandlerMove = ActionHandlerMove::GetSingleton();
		actionHandlerMove.yOffset = 1;
		return &actionHandlerMove;
	}
	if (dmwi::isPressed(dmwi::D))
	{
		ActionHandlerMove& actionHandlerMove = ActionHandlerMove::GetSingleton();
		actionHandlerMove.xOffset = 1;
		return &actionHandlerMove;
	}

	return nullptr;
}
