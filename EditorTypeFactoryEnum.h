#pragma once

#include "EditorTypeFactoryBase.h"

class EditorTypeFactoryEnum : public EditorTypeFactoryBase
{
public:
	//~ Begin EditorTypeFactoryBase Interface
	EditorTypeBase* CreateType(std::ifstream& stream) override;
	std::string GetKeyword() const override;
	//~ End EditorTypeFactoryBase Interface
};
