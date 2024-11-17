#pragma once

#include "EditorTypePropertyBase.h"

/** EditorTypePropertyClass
*
* This class is used to represent a class property (MyClass* pMyClass = nullptr)
* Classes are either instanced, meaning two pointers to the same data asset will result in two *copies* of the same class, not two pointers to same class.
* or they can be singletons 
*/
class EditorTypePropertyClass : public EditorTypePropertyBase
{
public:
	EditorTypePropertyClass() {}
	EditorTypePropertyClass(const std::string& propertyName, const std::string& inClassName, const std::string& inAssetName) : EditorTypePropertyBase(propertyName), className(inClassName), assetName(inAssetName) {}

	//~ Begin EditorTypePropertyBase Interface
	void DrawImGUI() override;
	EditorTypePropertyBase* DeepCopy() override;
	bool CanReadFromFile(std::ifstream& file) const override;
	void ReadFromFile(std::ifstream& file) override;
	void WriteToFile(std::ofstream& file) override;
	void ForceSetValue(const std::string& newValue) override;
	//~ End EditorTypePropertyBase Interface

	void* GetValue() const;

protected:
	std::string className; // name of the class of the asset
	std::string assetName; // name of the actual asset
};

