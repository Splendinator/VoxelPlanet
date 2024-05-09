#pragma once 

#include "EditorTypeBase.h"

class EditorTypePropertyBase;
struct OnPropertyChangedParams;

/** EditorTypePropertiesBase
*
* This class is used as a base class for any editor type that wants a list of properties.
* EditorTypeProperties (classes and structs) are deep copied and *also* used to represent actual instances of classes, not just the template object (CDO)
*/
class EditorTypePropertiesBase : public EditorTypeBase
{
public:
	EditorTypePropertiesBase() : EditorTypeBase() { onPropertyChangedDelegate.Bind(this, &EditorTypePropertiesBase::OnPropertyChanged); }

	virtual ~EditorTypePropertiesBase();
	
	// Draw the properties to ImGUI so we can edit them in the editor.
	virtual void DrawImGUI();

	// Read and write from a file
	virtual void ReadFromFile(std::ifstream& file);
	virtual void WriteToFile(std::ofstream& file);

	// Deep copy this type
	virtual EditorTypeBase* DeepCopy() = 0;
	
	// Make sure to call this when you're finished setting pPropeties
	void OnPropertiesPopulated();

	// List of properties
	std::vector<EditorTypePropertyBase*> pProperties;

	// Fired when a property changes
	DelegateList<const OnPropertyChangedParams&> onPropertyChanged;

protected:

	// Deep copy these properties to pOther
	void DeepCopyProperties(EditorTypePropertiesBase* pOther);

	void OnPropertyChanged(const OnPropertyChangedParams& params);

	DelegateClass<EditorTypePropertiesBase, const OnPropertyChangedParams&> onPropertyChangedDelegate;

private:

};
