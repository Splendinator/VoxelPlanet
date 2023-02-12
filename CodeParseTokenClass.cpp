#include "pch.h"

#include "CodeParseTokenClass.h"

void CodeParseTokenClass::WriteToFile(std::ofstream& outputFile)
{
	outputFile << "Object " << className;
		
	if (baseClass != "")
	{
		outputFile << " " << baseClass;
	}	
}

