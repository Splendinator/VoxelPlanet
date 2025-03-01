#pragma once 

#include "EditorTypeBase.h"

class EditorTypePropertyBase;
struct OnPropertyChangedData;


/** EditorTypePropertiesBase
*
* This class is used as a base class for any assetManager type that wants a list of properties.
* EditorTypeProperties (classes and structs) are deep copied and *also* used to represent actual instances of classes, not just the template object (CDO)
*/
class EditorTypePropertiesBase : public EditorTypeBase
{
public:
	EditorTypePropertiesBase() : EditorTypeBase() { onPropertyChangedDelegate.Bind(this, &EditorTypePropertiesBase::OnPropertyChanged); }

	virtual ~EditorTypePropertiesBase();
	
	// Draw the properties to ImGUI so we can edit them in the assetManager.
	virtual void DrawImGUI();

	// Read and write from a file
	virtual void ReadFromFile(std::ifstream& file);
	virtual void WriteToFile(std::ofstream& file);

	// Deep copy this type
	virtual EditorTypeBase* DeepCopy() = 0;
	
	// Make sure to call this when you're finished setting pPropeties
	void OnPropertiesPopulated();

	// Fires when the metadataFlags are updated on the template assetManager type only.
	// Non-template assetManager types will just copy the metadata flags from the template.
	// This can be used to do any post processing on metadata flags, e.g inheriting flags from children.
	void OnTemplateMetadataFlagsPopulated();

	// List of properties
	std::vector<EditorTypePropertyBase*> pProperties;

	// Names of base classes if any. This will always be empty on deep copied versions as we only care about base classes on the template.
	std::vector<std::string> baseClasses;
	
	// Fired when a property changes
	DelegateList<const OnPropertyChangedData&> onPropertyChanged;

protected:

	// Deep copy these properties to pOther
	void DeepCopyProperties(EditorTypePropertiesBase* pOther);

	void OnPropertyChanged(const OnPropertyChangedData& params);

	// Recursively scan children template objects for the first of either Instanced or Singleton.
	// If none are found we return EClassMetadataFlags::None
	EClassMetadataFlags GetChildInstancedOrSingletonFlag(); 

	DelegateClass<EditorTypePropertiesBase, const OnPropertyChangedData&> onPropertyChangedDelegate;

private:

};
