#include "pch.h"

#include "RPGSkillAimModules.h"

// RPGSkillAimModuleLine
ERPGSkillHighlightType RPGSkillAimModuleLine::GetSkillHighlightTypeForLocation(const RPGSkillParams& params, const Vec2i& location) const
{
	// #TEMP: Do projectile line
	
	if (params.targetPos == location)
	{
		return (Vec2i::Distance(params.startPos, params.targetPos) <= (float)maxRange) ? ERPGSkillHighlightType::Valid : ERPGSkillHighlightType::Invalid; 
	}
		
	return Vec2i::Distance(location, params.targetPos) <= (float)areaRadius ? ERPGSkillHighlightType::AffectedArea : ERPGSkillHighlightType::None; 
}

bool RPGSkillAimModuleLine::IsAimValid(const RPGSkillParams& params) const
{
	return Vec2i::Distance(params.startPos, params.targetPos) <= (float)maxRange;
}
