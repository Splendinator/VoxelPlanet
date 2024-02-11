#pragma once

#include "EditorTypePropertiesBase.h"

/** EditorTypeStruct
*
* This class used for editor type structs. These are full structs with a list of properties, not a "reference to" a struct.
* For structs, this class is only used internally by EditorPropertyTypeStruct. (i.e used to inline struct properties in class assets)
* This is slightly different from how EditorTypeClass is used. 
*/
class EditorTypeStruct : public EditorTypePropertiesBase
{
public:
	EditorTypeStruct() = default;

	//~ Begin EditorTypeBase Interface
	void DrawImGUI() override;
	EditorTypeBase* DeepCopy() override;
	virtual void ReadFromFile(std::ifstream& file);
	virtual void WriteToFile(std::ofstream& file);
	//~ End EditorTypeBase Interface

};
