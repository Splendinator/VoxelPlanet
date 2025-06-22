#pragma once

#include "ActionDeciderBase.h"

class TextRenderSystem;

// Okay, this class needs explaining.
// Because I'm using ECS for this fucking project even though it's a turn based roguelike there's a lot of bullshit that I need to do to make it work.
// The ECSSystemAction is there to handle things taking turns, it was originally supposed to be used for players/AI but because I need
// Status effects to tick once per turn I can just re-use the action system to make status effects happen.
EDITORCLASS()
class ActionDeciderStatusEffect : public ActionDeciderBase
{
	EDITORBODY()
	
public:
	//~ Begin ActionDeciderBase Interface
	ActionHandlerBase* DecideAction(ECS& ecs, EntityId actor) override;
	//~ End ActionDeciderBase Interface
	
};
