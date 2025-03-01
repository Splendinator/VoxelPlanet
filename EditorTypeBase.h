#pragma once

// Class metadata flags that can go inside EDITORCLASS() or EDITORSTRUCT()
enum class EClassMetadataFlags
{
	None = 0,
	Abstract = 1 << 0,		// Cannot make data assets of this class
	Instanced = 1 << 1,		// When there's multiple pointers to the same data asset each one will resolve to pointing to their own instance of that object   
	Singleton = 1 << 2, 	// When there's multiple pointers to the same data asset each one will resolve to the same object
	EditInlineNew = 1 << 3, // When creating a property pointer to this class, instead of linking to an external asset you'll create one inline like a struct
};

/** EditorTypeBase
*
* This class the base class for all assetManager types (class/struct/enum...).
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

	bool HasMetadataFlag(EClassMetadataFlags flag);
	void AddMetadataFlag(EClassMetadataFlags flag);
	
	// #TODO: Make this protected
	// Name of this type. (e.g the name of the class)
	std::string name;

	// Engine level metadata flags
	EClassMetadataFlags metadataFlags = EClassMetadataFlags::None;
	
	
};
