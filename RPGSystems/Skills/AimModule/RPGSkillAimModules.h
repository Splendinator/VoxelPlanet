#pragma once

#include "RPGSystems/Skills/RPGSkillsShared.h"

// Aimimg module of a skill, determines which squares you can aim at.
// You can combine these modules to create desired effects (only able to aim at allies, only able to aim within 3 squares)
EDITORCLASS(Abstract, EditInlineNew)
class RPGSkillAimModuleBase
{
	EDITORBODY()
public:

	virtual ERPGSkillAimResponse CanAimSkill(const RPGSkillParams& params) { PUREVIRTUAL() return ERPGSkillAimResponse::Invalid; }
};

// #TEMP: 
EDITORCLASS()
class RPGSkillAimModuleTemp : public RPGSkillAimModuleBase
{
	EDITORBODY()

	ERPGSkillAimResponse CanAimSkill(const RPGSkillParams& params) override { return ERPGSkillAimResponse::Valid; }
};