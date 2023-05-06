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

	// List of properties
	std::vector<EditorTypePropertyBase*> pProperties;

	//~ Begin EditorTypeBase Interface
	void DrawImGUI() override;
	//~ End EditorTypeBase Interface
	
protected:

private:

};
