#pragma once

#include "EditorTypePropertyFactoryBase.h"

class EditorTypePropertyFactoryVector : public EditorTypePropertyFactoryBase
{
public:
	//~ Begin EditorTypePropertyFactoryBase Interface
	virtual EditorTypePropertyBase* CreateType(std::ifstream& stream) override;
	virtual std::string GetLabel() override { return "vector"; } 
	//~ End EditorTypePropertyFactoryBase Interface
};
