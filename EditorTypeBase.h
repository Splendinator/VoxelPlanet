#pragma once

struct OnPropertyChangedParams;

/** EditorTypeBase
*
* This class the base class for all editor types (class/struct/enum...).
* Editor types contain a list of properties and their values.
* They are used for 2 things:
* 1. On assets in the game to keep track of their properties (in this case they are instanced per asset)
* 2. In the "template" class types (ImGuiEditor::templateTypes) to keep track of default values for a class
*/
class EditorTypeBase
{
public:
	EditorTypeBase() {};
	virtual ~EditorTypeBase() {};
	
	// #TODO: Make this protected
	// Name of this type. (e.g the name of the class)
	std::string name;

};
