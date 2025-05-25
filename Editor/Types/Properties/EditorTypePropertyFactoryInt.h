#pragma once

#include "EditorTypePropertyFactoryBase.h"

/** EditorTypePropertyFactoryInt
*
* This class is used for generating a EditorTypePropertyInt
*/
class EditorTypePropertyFactoryInt : public EditorTypePropertyFactoryBase
{
public:
	//~ Begin EditorTypePropertyFactoryBase Interface
	virtual EditorTypePropertyBase* CreateType(std::ifstream& stream) override;
	virtual EditorTypePropertyBase* CreateTemplateType() override;
	virtual std::string GetLabel() override;
	//~ End EditorTypePropertyFactoryBase Interface
};
