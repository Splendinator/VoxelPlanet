#pragma once

#include "CodeParseTokenPropertyBase.h"

/** CodeParseTokenPropertyFloat
*
* This class is used for float properties
*/
class CodeParseTokenPropertyFloat : public CodeParseTokenPropertyBase
{
public:

	CodeParseTokenPropertyFloat(std::string inPropertyName, std::string inDefaultValue) : CodeParseTokenPropertyBase(inPropertyName), defaultValue(inDefaultValue) {}

	//~ Begin CodeParseTokenBase Interface
	virtual void WriteToFile(std::ofstream& outputFile) override;
	//~ End CodeParseTokenBase Interface

private:

	std::string defaultValue;
};
