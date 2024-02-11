#pragma once

#include "EditorTypePropertyFactoryBase.h"

/** EditorTypePropertyFactoryStruct
*
* This struct is used for generating a EditorTypePropertyStruct
*/
class EditorTypePropertyFactoryStruct : public EditorTypePropertyFactoryBase
{
public:
	//~ Begin EditorTypePropertyFactoryBase Interface
	virtual EditorTypePropertyBase* CreateType(std::ifstream& stream) override;
	virtual std::string GetLabel() override;
	//~ End EditorTypePropertyFactoryBase Interface
};
