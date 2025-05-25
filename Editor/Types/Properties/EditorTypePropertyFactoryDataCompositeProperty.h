#pragma once

#include "EditorTypePropertyFactoryBase.h"

/** EditorTypePropertyFactoryDataCompositeProperty
*
* This class is used for generating a EditorTypePropertyDataCompositeProperty
*/
class EditorTypePropertyFactoryDataCompositeProperty : public EditorTypePropertyFactoryBase
{
public:
	//~ Begin EditorTypePropertyFactoryBase Interface
	virtual EditorTypePropertyBase* CreateType(std::ifstream& stream) override;
	virtual std::string GetLabel() override;
	//~ End EditorTypePropertyFactoryBase Interface
};