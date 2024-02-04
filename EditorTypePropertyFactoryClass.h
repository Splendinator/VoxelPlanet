#pragma once

#include "EditorTypePropertyFactoryBase.h"

/** EditorTypePropertyFactoryClass
*
* This class is used for generating a EditorTypePropertyClass
*/
class EditorTypePropertyFactoryClass : public EditorTypePropertyFactoryBase
{
public:
	//~ Begin EditorTypePropertyFactoryBase Interface
	virtual EditorTypePropertyBase* CreateType(std::ifstream& stream) override;
	virtual std::string GetLabel() override;
	//~ End EditorTypePropertyFactoryBase Interface
};
