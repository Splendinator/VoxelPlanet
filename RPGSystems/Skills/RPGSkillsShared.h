#pragma once

#include "ECS/ECSTypes.h"

class ECS;
class ECSSystemEntityMap;
class RPGSystem;

// When trying to aim a skill at a specific location it will return one of these values for that location.
// e.g. aiming a 5 range fireball at 6 range will return a ERPGSkillHighlightType::Invalid to show you can't aim there .
EDITORENUM()
enum class ERPGSkillHighlightType : u8
{
	None,			// Don't show anything on this square
	Valid,			// Green highlight typically used to show valid aiming square(s)
	Invalid,		// Red highlight typically used to show invalid aiming square(s)
	AffectedArea,	// Yellow highlight typically used to show the square will be affected in some way. 
};

// Params passed to all skill conditions / effects.
// Anything frequently wanted by lots of skills should go here to save time
struct RPGSkillParams
{
	ECS& ecs;
	ECSSystemEntityMap& entityMapSystem;
	RPGSystem& rpgSystem;
	
	EntityId caster;
	
	Vec2i startPos; // Usually where the caster is standing
	Vec2i targetPos; // Where the skill is being aimed at
};
