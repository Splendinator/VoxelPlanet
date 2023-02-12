#pragma once

/** CodeParseTokenBase
*
* This class is used as a token for reading in code files. It represents an useful part of a code file for the purpose of gathering data to be used in the ImGuiEditor (see ImGuiEditor.h)
* A token's primary purpose is to write useful data (class name, property name+default value, etc.) to a master file.
*/
class CodeParseTokenBase
{
public:

	// Override to write this token to a file. It is assumed this token will create a single line in the file so don't add std::endl at the end.
	virtual void WriteToFile(std::ofstream& outputFile) = 0;
};
