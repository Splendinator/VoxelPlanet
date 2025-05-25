#pragma once

class EditorTypeBase;

/** EditorTypeFactoryBase
*
* This class is the base class used to create assetManager types. See EditorType.
* This creates assetManager types based off an input file.
* This is currently only used to generate "template types". See ImGuiEditor::CreateTemplateTypes()
*/
class EditorTypeFactoryBase
{
public:

	// Should be overridden to create an assetManager type based off the input
	virtual EditorTypeBase* CreateType(std::ifstream& stream) = 0;

	// The keyword that this factory looks for before creating a type
	virtual std::string GetKeyword() const = 0;

};
