#pragma once

#include "EditorTypePropertyBase.h"

/** 
 *  This class is used to represent an enum property
 */
class EditorTypePropertyEnum : public EditorTypePropertyBase
{
	
public:
	EditorTypePropertyEnum() : EditorTypePropertyBase() {}
	EditorTypePropertyEnum(const std::string& propertyName, const std::string& inEnumName, int inValue) : EditorTypePropertyBase(propertyName), enumName(inEnumName), value(inValue) {}

	//~ Begin EditorTypePropertyBase Interface
	void DrawImGUI() override;
	EditorTypePropertyBase* DeepCopy() override;
	bool CanReadFromFile(std::ifstream& file) const override;
	void ReadFromFile(std::ifstream& file) override;
	void WriteToFile(std::ofstream& file) override;
	void ForceSetValue(const std::string& newValue) override;
	//~ End EditorTypePropertyBase Interface

	int GetValue() const { return value; }
	std::string GetEnumName() { return enumName; }

protected:
	int value = 0;
	std::string enumName;
};
