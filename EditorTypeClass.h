#pragma once

#include "EditorTypePropertiesBase.h"

/** EditorTypeClass
*
* This class used for editor type classes. These are full classes with a list of properties, not a "reference to" a class
*/
class EditorTypeClass : public EditorTypePropertiesBase
{
public:
	EditorTypeClass() = default;

	//~ Begin EditorTypeBase Interface
	void DrawImGUI() override;
	EditorTypeBase* DeepCopy() override;
	virtual void ReadFromFile(std::ifstream& file);
	virtual void WriteToFile(std::ofstream& file);
	//~ End EditorTypeBase Interface

protected:

private:

};
