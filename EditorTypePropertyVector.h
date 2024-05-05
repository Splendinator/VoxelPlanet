#pragma once

#include "EditorTypePropertyBase.h"

/** EditorTypePropertyVector
 *
 * Editor type for a vector property of any type
 */
class EditorTypePropertyVector : public EditorTypePropertyBase
{
public:
	EditorTypePropertyVector(EditorTypePropertyBase* templateType, const std::string& inName);

	//~ Begin EditorTypePropertyBase Interface
	void DrawImGUI() override;
	bool CanReadFromFile(std::ifstream& file) const override;
	void ReadFromFile(std::ifstream& file) override;
	void WriteToFile(std::ofstream& file) override;
	EditorTypePropertyBase* DeepCopy() override;
	void ForceSetValue(const std::string& newValue) override;
	//~ End EditorTypePropertyBase Interface

	std::unique_ptr<EditorTypePropertyBase> dataTemplateType; // The property data type that are in the vector (PropertyTypeFloat for std::vector<float> etc.)

	std::vector<std::unique_ptr<EditorTypePropertyBase>> instancedProperties; // The instantiation of the template type ({PropertyTypeFloat(3.0f), PropertyTypeFloat(5.0f)} etc.)

	int arrayLength = 0;

protected:

	DelegateClass<EditorTypePropertyVector, const OnPropertyChangedParams&> onInternalVectorPropertyChanged;
	
	void OnInternalVectorPropertyChanged(const OnPropertyChangedParams& onPropertyChangedParams);

	void AddEntry(EditorTypePropertyBase* pEntry);
	void AddDefaultEntry();
};
