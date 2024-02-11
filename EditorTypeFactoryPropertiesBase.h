#pragma once

#include "EditorTypeFactoryBase.h"

class EditorTypePropertiesBase;
class EditorTypePropertyBase;

/** EditorTypeFactoryPropertiesBase
*
* This class is used as a base class for creating any editor type that wants a list of properties.
*/
class EditorTypeFactoryPropertiesBase : public EditorTypeFactoryBase
{
protected:

	void PopulateProperties(std::ifstream& stream, EditorTypePropertiesBase* pEditorTypeProperties);

};
