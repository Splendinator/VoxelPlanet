#pragma once

#include "ECS/ECSTypes.h"

class ECS;
class ECSSystemEntityMap;

// When trying to aim a skill at a specific location it will return one of these values for that location.
// e.g. aiming a 5 range fireball at 6 range will return a ERPGSkillAimResponse::Invalid to show you can't aim there .
EDITORENUM()
enum class ERPGSkillAimResponse : u8
{
	Valid,
	Invalid,
};

struct RPGSkillParams
{
	ECS& ecs;
	ECSSystemEntityMap& entityMapSystem;
	
	EntityId caster;
	
	Vec2i startPos; // Usually where the caster is standing
	Vec2i targetPos; // Where the skill is being aimed at
};