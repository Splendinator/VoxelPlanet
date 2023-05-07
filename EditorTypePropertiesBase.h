#pragma once 

#include "EditorTypeBase.h"

class EditorTypePropertyBase;

/** EditorTypePropertiesBase
*
* This class is used as a base class for any editor type that wants a list of properties.
*/
class EditorTypePropertiesBase : public EditorTypeBase
{
public:

	virtual ~EditorTypePropertiesBase();

	// List of properties
	std::vector<EditorTypePropertyBase*> pProperties;

	//~ Begin EditorTypeBase Interface
	void DrawImGUI() override;
	//~ End EditorTypeBase Interface
	
protected:

	// Deep copy these properties to pOther
	void DeepCopyProperties(EditorTypePropertiesBase* pOther);

private:

};
