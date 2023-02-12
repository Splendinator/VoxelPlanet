#pragma once

#include "CodeParseTokenFactoryBase.h"

/** CodeParseTokenFactoryClass
*
* This class is used to create CodeParseTokenClass objects
*/
class CodeParseTokenFactoryClass : public CodeParseTokenFactoryBase
{
public:
	
	//~ Begin CodeParseTokenFactoryBase Interface
	bool IsKeyword(const std::string& keyword) override;
	CodeParseTokenBase* CreateToken(const std::string& keyword, std::stringstream& stream) override;
	std::string GetEndString() const override;
	//~ End CodeParseTokenFactoryBase Interface

	protected:
	
	private:

};
