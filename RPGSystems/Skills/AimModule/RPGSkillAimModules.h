#pragma once

#include "RPGSystems/Skills/RPGSkillsShared.h"

// Aimimg module of a skill, determines which squares you can aim at.
// You can combine these modules to create desired effects (only able to aim at allies, only able to aim within 3 squares)
EDITORCLASS(Abstract, EditInlineNew)
class RPGSkillAimModuleBase
{
	EDITORBODY()

public:
	
	// This returns the type of highlight we want at a given location, and will be unique to the skill being aimed.
	// For example, with a fireball we might want to draw a green line showing the fireball projectile trajectory and a yellow circle around explosion area.
	// params.startPos/Endpos will be where the player standing and trying to aim, location will be all squares on the screen around the player
	virtual ERPGSkillHighlightType GetSkillHighlightTypeForLocation(const RPGSkillParams& params, const Vec2i& location) const { PUREVIRTUAL() return ERPGSkillHighlightType::None; }

	// This should return whether the skill can be aimed at params.targetPos
	virtual bool IsAimValid(const RPGSkillParams& params) const { PUREVIRTUAL() return false; }
	
};

// Aiming in a line up to a max range with optional area radius affected around the target location
EDITORCLASS()
class RPGSkillAimModuleLine : public RPGSkillAimModuleBase
{
	EDITORBODY()

protected:

	//~ Begin RPGSkillAimModuleBase Interface
	ERPGSkillHighlightType GetSkillHighlightTypeForLocation(const RPGSkillParams& params, const Vec2i& location) const override;
	bool IsAimValid(const RPGSkillParams& params) const override;
	//~ End RPGSkillAimModuleBase Interface
	
	// Range the projectile can be aimed
	EDITORPROPERTY()
	u32 maxRange = 0;

	// Area around target location the projectile will affect
	EDITORPROPERTY()
	u32 areaRadius = 0;
};