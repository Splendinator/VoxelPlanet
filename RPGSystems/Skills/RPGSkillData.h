#pragma once

class RPGSkillEffectModuleBase;
class RPGSkillAimModuleBase;

// Data asset describing a single spell
EDITORCLASS(Singleton)
class RPGSkillData
{
	EDITORBODY()
public:

	EDITORPROPERTY()
	const RPGSkillAimModuleBase* pAimModule;

	EDITORPROPERTY()
	const RPGSkillEffectModuleBase* pEffectModule;
	
};
