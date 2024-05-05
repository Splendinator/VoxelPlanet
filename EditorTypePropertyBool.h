#pragma once

#include "EditorTypePropertyBase.h"

/** EditorTypePropertyBool
*
* This class is used to represent a boolean property
*/
class EditorTypePropertyBool : public EditorTypePropertyBase
{
public:
	EditorTypePropertyBool() : EditorTypePropertyBase() {}
	EditorTypePropertyBool(const std::string& propertyName, bool inValue) : EditorTypePropertyBase(propertyName), value(inValue) {}

	//~ Begin EditorTypePropertyBase Interface
	void DrawImGUI() override;
	EditorTypePropertyBase* DeepCopy() override;
	bool CanReadFromFile(std::ifstream& file) const override;
	void ReadFromFile(std::ifstream& file) override;
	void WriteToFile(std::ofstream& file) override;
	void ForceSetValue(const std::string& newValue) override;
	//~ End EditorTypePropertyBase Interface

	bool GetValue() const { return value; }

protected:
	bool value = false;
};
