#pragma once

#include "EditorTypePropertyBase.h"

/** EditorTypePropertyFloat
*
* This class is used to represent a float number property
*/
class EditorTypePropertyFloat : public EditorTypePropertyBase
{
public:
	EditorTypePropertyFloat(const std::string& propertyName, float inValue) : EditorTypePropertyBase(propertyName), value(inValue) {}

	//~ Begin EditorTypePropertyBase Interface
	void DrawImGUI() override;
	//~ End EditorTypePropertyBase Interface

protected:
	float value = 0.0f;
};