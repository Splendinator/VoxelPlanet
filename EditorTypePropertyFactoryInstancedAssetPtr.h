#pragma once

#include "EditorTypePropertyFactoryBase.h"

/** EditorTypePropertyFactoryInstancedAssetPtr
*
* This class is used for generating a EditorTypePropertyInstancedAssetPtr
*/
class EditorTypePropertyFactoryInstancedAssetPtr : public EditorTypePropertyFactoryBase
{
public:
	//~ Begin EditorTypePropertyFactoryBase Interface
	virtual EditorTypePropertyBase* CreateType(std::ifstream& stream) override;
	virtual std::string GetLabel() override;
	//~ End EditorTypePropertyFactoryBase Interface
};