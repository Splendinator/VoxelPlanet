#pragma once

#include "EditorTypePropertyFactoryBase.h"

class EditorTypePropertyFactoryEnum : public EditorTypePropertyFactoryBase
{
public:
	//~ Begin EditorTypePropertyFactoryBase Interface
	EditorTypePropertyBase* CreateType(std::ifstream& stream) override;
	std::string GetLabel() override;
	//~ End EditorTypePropertyFactoryBase Interface
	
};
