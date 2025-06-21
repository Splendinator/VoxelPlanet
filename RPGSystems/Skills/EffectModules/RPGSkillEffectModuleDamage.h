#include "RPGSkillEffectModuleBase.h"

#include "RPGSystems/RPGSystem.h"

EDITORCLASS()
class RPGSkillEffectModuleDamage : public RPGSkillEffectModuleBase
{
    EDITORBODY()
public:
    //~ Begin RPGSkillDamageEffectModule Interface
    void ApplyEffect(const RPGSkillParams& params) const override;
    //~ End RPGSkillDamageEffectModule Interface

	EDITORPROPERTY()
	DataCompositeProperty<RPGDamageMagnitude> damage;
};