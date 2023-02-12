#pragma once

#include "CodeParseTokenFactoryBase.h"

/** CodeParseTokenFactoryProperty
*
* This class creates a property token
*/
class CodeParseTokenFactoryProperty : public CodeParseTokenFactoryBase
{
public:

	//~ Begin CodeParseTokenFactoryBase Interface
	bool IsKeyword(const std::string& keyword) override;
	CodeParseTokenBase* CreateToken(const std::string& keyword, std::stringstream& stream) override;
	//~ End CodeParseTokenFactoryBase Interface
};
