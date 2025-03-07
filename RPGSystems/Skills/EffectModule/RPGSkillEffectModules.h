#pragma once

#include "RPGSystems/Skills/RPGSkillsShared.h"

// Effect applied on 
EDITORCLASS(Abstract, EditInlineNew)
class RPGSkillEffectModuleBase
{
	EDITORBODY()
public:

	virtual void ApplyEffect(const RPGSkillParams& params) const { PUREVIRTUAL() }
};

EDITORCLASS()
class RPGSkillEffectModuleTemp : public RPGSkillEffectModuleBase
{
	EDITORBODY()
public:
	void ApplyEffect(const RPGSkillParams& params) const override;

	EDITORPROPERTY()
	u32 damage = 0;
};
