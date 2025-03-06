#pragma once

#include "RPGSystems/Skills/RPGSkillsShared.h"

// Effect applied on 
EDITORCLASS(Abstract, EditInlineNew)
class RPGSkillEffectModuleBase
{
	EDITORBODY()
public:

	virtual void ApplyEffect(const RPGSkillParams& params) { PUREVIRTUAL() }
};

EDITORCLASS()
class RPGSkillEffectModuleTemp : public RPGSkillEffectModuleBase
{
	EDITORBODY()
public:
	void ApplyEffect(const RPGSkillParams& params) override;

	EDITORPROPERTY()
	u32 damage = 0;
};
