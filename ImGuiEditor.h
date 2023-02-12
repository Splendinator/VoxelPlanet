#pragma once

class CodeParseTokenBase;

/** ImGuiEditor
*
* This class is used to edit the game in using ImGui. 
*
* It reads in all the code files in the project and parses them for classes, structs, enums, etc and lets you edit them.
*/
class ImGuiEditor
{
public:
	void Init();
	void Uninit();
	void Tick();

private:
	
	static std::vector<std::string> GetCodeFilesInDirectory(const std::string& directory);

	void Parse(std::string codeFile, std::vector<CodeParseTokenBase*>& allTokens);
};
