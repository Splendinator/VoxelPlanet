#include "RPGSkillAimModuleBase.h"


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
	DataCompositeProperty<int> maxRange = 0;

	// Area around target location the projectile will affect
	EDITORPROPERTY()
	DataCompositeProperty<int> areaRadius = 2;
};