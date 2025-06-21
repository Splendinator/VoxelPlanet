#pragma once

#include "RPGSkillEffectModuleBase.h"

class ActionDeciderProjectile;

EDITORCLASS()
class RPGSkillEffectModuleProjectile : public RPGSkillEffectModuleBase
{
	EDITORBODY()
public:
	//~ Begin RPGSkillEffectModuleBase Interface
	void ApplyEffect(const RPGSkillParams& params) const override;
	//~ End RPGSkillEffectModuleBase Interface

	EDITORPROPERTY()
	std::string projectileFileName;
	
	EDITORPROPERTY()
	ActionDeciderProjectile* pActionDecider = nullptr;
	
	// Speed of projectile, in tiles per turn (100 energy = 1 turn)
	EDITORPROPERTY()
	DataCompositeProperty<float> speed;

	EDITORPROPERTY()
	RPGSkillEffectModuleBase* pProjectileEffect = nullptr;
};
