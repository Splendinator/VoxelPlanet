#include "pch.h"

#include "RPGSkillAimModules.h"

// RPGSkillAimModuleLine
ERPGSkillHighlightType RPGSkillAimModuleLine::GetSkillHighlightTypeForLocation(const RPGSkillParams& params, const Vec2i& location) const
{
	// #TODO: Do projectile line (make a line util)
	// #TODO: Make the +0.25f for visually pleasing circles a thing (also make it a util)

	if (params.targetPos == location)
	{
		return (Vec2i::Distance(params.startPos, params.targetPos) < (float)maxRange + 0.25f) ? ERPGSkillHighlightType::Valid : ERPGSkillHighlightType::Invalid; 
	}
		
	return Vec2i::Distance(location, params.targetPos) < (float)areaRadius + 0.25f ? ERPGSkillHighlightType::AffectedArea : ERPGSkillHighlightType::None; 
}

bool RPGSkillAimModuleLine::IsAimValid(const RPGSkillParams& params) const
{
	return Vec2i::Distance(params.startPos, params.targetPos) < (float)maxRange + 0.25f;
}
