#include "pch.h"

#include "CodeParseTokenPropertyFloat.h"

void CodeParseTokenPropertyFloat::WriteToFile(std::ofstream& outputFile)
{
	outputFile << "float ";
	CodeParseTokenPropertyBase::WriteToFile(outputFile);
	outputFile << " " << defaultValue;
}
