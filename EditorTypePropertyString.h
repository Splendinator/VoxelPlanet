#pragma once

#include "EditorTypePropertyBase.h"

/** EditorTypePropertyString
*
* This class is used to represent a string property
*/
class EditorTypePropertyString : public EditorTypePropertyBase
{
public:
	EditorTypePropertyString() : EditorTypePropertyBase() {}
	EditorTypePropertyString(const std::string& propertyName, const std::string& inValue) : EditorTypePropertyBase(propertyName), value(inValue) {}

	//~ Begin EditorTypePropertyBase Interface
	void DrawImGUI() override;
	EditorTypePropertyBase* DeepCopy() override;
	bool CanReadFromFile(std::ifstream& file) const override;
	void ReadFromFile(std::ifstream& file) override;
	void WriteToFile(std::ofstream& file) override;
	void ForceSetValue(const std::string& newValue) override;
	//~ End EditorTypePropertyBase Interface

	std::string GetValue() const { return value; }

protected:
	std::string value = "";
};
