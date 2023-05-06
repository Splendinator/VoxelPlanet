#pragma once

class CodeParseTokenBase;
class EditorTypeBase;

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
	
	// Generate template types. see templateTypes
	void CreateTemplateTypes(const std::string& typesFile);

	// This map will contain a single instance of each type (the key being it's name) and all of the EditorTypes will have default values.
	// i.e "SpellFireball" -> "EditorTypeClass(SpellFireball) with all the default numbers"
	std::unordered_map<std::string, EditorTypeBase*> templateTypes;

};
