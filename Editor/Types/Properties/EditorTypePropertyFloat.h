#pragma once

#include "EditorTypePropertyBase.h"

/** EditorTypePropertyFloat
*
* This class is used to represent a float number property
*/
class EditorTypePropertyFloat : public EditorTypePropertyBase
{
public:
	EditorTypePropertyFloat() : EditorTypePropertyBase() {}
	EditorTypePropertyFloat(const std::string& propertyName, float inValue) : EditorTypePropertyBase(propertyName), value(inValue) {}
	//~ Begin EditorTypePropertyBase Interface
	void DrawImGUI() override;
	EditorTypePropertyBase* DeepCopy() override;
	bool CanReadFromFile(std::ifstream& file) const override;
	void ReadFromFile(std::ifstream& file) override;
	void WriteToFile(std::ofstream& file) override;
	void ForceSetValue(const ForceSetValueParams& params) override;
	//~ End EditorTypePropertyBase Interface

	float GetValue() const { return value; }

protected:
	float value = 0.0f;
};