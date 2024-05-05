#pragma once

#include "EditorTypePropertyBase.h"

/** EditorTypePropertyInt
*
* This class is used to represent an integer property
*/
class EditorTypePropertyInt : public EditorTypePropertyBase
{
public:
	EditorTypePropertyInt() : EditorTypePropertyBase() {}
	EditorTypePropertyInt(const std::string& propertyName, int inValue) : EditorTypePropertyBase(propertyName), value(inValue) {}

	//~ Begin EditorTypePropertyBase Interface
	void DrawImGUI() override;
	EditorTypePropertyBase* DeepCopy() override;
	bool CanReadFromFile(std::ifstream& file) const override;
	void ReadFromFile(std::ifstream& file) override;
	void WriteToFile(std::ofstream& file) override;
	void ForceSetValue(const std::string& newValue) override;
	//~ End EditorTypePropertyBase Interface

	int GetValue() const { return value; }

protected:
	int value = 0;
};
