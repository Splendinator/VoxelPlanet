#pragma once

#include "EditorTypePropertyBase.h"

class EditorTypeStruct;

/** EditorTypePropertyStruct
*
* This struct is used to represent a struct property ("MyStruct myStruct;")
*/
class EditorTypePropertyStruct : public EditorTypePropertyBase
{
public:
	EditorTypePropertyStruct(const std::string& propertyName, const std::string& inStructName, EditorTypeStruct* pInStructType);

	//~ Begin EditorTypePropertyBase Interface
	void DrawImGUI() override;
	EditorTypePropertyBase* DeepCopy() override;
	void ReadFromFile(std::ifstream& file) override;
	void WriteToFile(std::ofstream& file) override;
	void ForceSetValue(const std::string& newValue) override;
	//~ End EditorTypePropertyBase Interface

	void* GetValue() const;

protected:
	std::string structName; // name of the struct. i.e "Vec3f" not "position"

	std::unique_ptr<EditorTypeStruct> pStructType; // Deep copied struct type. This contains all the properties etc.
	
	DelegateClass<EditorTypePropertyStruct, const OnPropertyChangedParams&> onInternalStructPropertyChangedDelegate;

	void OnInternalStructPropertyChanged(const OnPropertyChangedParams& onPropertyChangedParams);
};

