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
class RPGSkillDamageEffectModule : public RPGSkillEffectModuleBase
{
    EDITORBODY()
public:
    //~ Begin RPGSkillDamageEffectModule Interface
    void ApplyEffect(const RPGSkillParams& params) const override;
    //~ End RPGSkillDamageEffectModule Interface
};

