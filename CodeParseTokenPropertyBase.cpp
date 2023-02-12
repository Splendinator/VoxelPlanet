#include "pch.h"

#include "CodeParseTokenPropertyBase.h"

void CodeParseTokenPropertyBase::WriteToFile(std::ofstream& outputFile)
{
	outputFile << propertyName;
}
