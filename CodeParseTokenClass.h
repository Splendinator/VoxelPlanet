#pragma once

#include "CodeParseTokenBase.h"

/** CodeParseTokenClass
*
* Token for a class
*/
class CodeParseTokenClass : public CodeParseTokenBase
{
public:

	CodeParseTokenClass(std::string inClassName) : CodeParseTokenBase(), className(inClassName) {}
	CodeParseTokenClass(std::string inClassName, std::string inBaseClass) : CodeParseTokenBase(), className(inClassName), baseClass(inBaseClass) {}
	
	//~ Begin CodeParseTokenBase Interface
	virtual void WriteToFile(std::ofstream& outputFile) override;
	//~ End CodeParseTokenBase Interface
private:

	std::string className;
	std::string baseClass;
};