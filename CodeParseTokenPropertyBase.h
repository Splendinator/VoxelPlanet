#pragma once

#include "CodeParseTokenBase.h"

/** CodeParseTokenPropertyBase
*
* Base class used for property tokens
*/
class CodeParseTokenPropertyBase : public CodeParseTokenBase
{
public:
	CodeParseTokenPropertyBase(std::string inPropertyName) : CodeParseTokenBase(), propertyName(inPropertyName) {}

	//~ Begin CodeParseTokenBase Interface
	virtual void WriteToFile(std::ofstream& outputFile) override;
	//~ End CodeParseTokenBase Interface

protected:

	std::string propertyName;
};