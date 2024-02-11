#pragma once

#include "EditorTypeFactoryPropertiesBase.h"

/** EditorTypeFactoryStruct
*
* This struct is used for creating a struct
*/
class EditorTypeFactoryStruct : public EditorTypeFactoryPropertiesBase
{
public:

	//~ Begin EditorTypeFactoryBase Interface
	EditorTypeBase* CreateType(std::ifstream& stream) override;
	std::string GetKeyword() const override;
	//~ End EditorTypeFactoryBase Interface
};
