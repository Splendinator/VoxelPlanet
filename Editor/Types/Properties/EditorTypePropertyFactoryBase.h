#pragma once

class EditorTypePropertyBase;

/** EditorTypePropertyFactoryBase
*
* This class is used to generate editor property types.
* These are only used when generating "template types" (See ImGuiEditor::CreateTemplateTypes())
*/
class EditorTypePropertyFactoryBase
{
public:
	virtual ~EditorTypePropertyFactoryBase() = default;

	// Should be overwritten to create an editor property type from an input stream
	virtual EditorTypePropertyBase* CreateType(std::ifstream& stream) = 0;

	// Should be overwritten to create an editor property type with default values if such a concept exists
	// (i.e floats can be made with 0.0f, but structs *need* a reference struct to copy from (i.e ImGuiEditor::FindStructTemplateType) 
	virtual EditorTypePropertyBase* CreateTemplateType() { return nullptr; }
	
	// Should be overwritten to return the keyword for this type
	virtual std::string GetLabel() = 0;

};
