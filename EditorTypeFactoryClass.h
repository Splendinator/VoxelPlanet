#pragma once

#include "EditorTypeFactoryPropertiesBase.h"

/** EditorTypeFactoryClass
*
* This class is used for creating a class
*/
class EditorTypeFactoryClass : public EditorTypeFactoryPropertiesBase
{
public:

	//~ Begin EditorTypeFactoryBase Interface
	EditorTypeBase* CreateType(std::ifstream& stream) override;
	std::string GetKeyword() const override;
	//~ End EditorTypeFactoryBase Interface

protected:

private:

};
