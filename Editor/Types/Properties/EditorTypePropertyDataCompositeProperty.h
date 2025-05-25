#pragma once

#include "EditorTypePropertyBase.h"

/** EditorTypePropertyDataCompositeProperty
*
* This class represents an EDITORPROPERTY() that is a DataCompositeProperty<T>
*/
class EditorTypePropertyDataCompositeProperty : public EditorTypePropertyBase
{
public:
	EditorTypePropertyDataCompositeProperty() = default;
	EditorTypePropertyDataCompositeProperty(const std::string& propertyName, const std::string& inDataType, const std::string& inAssetName, const std::string& inAssetPropertyName)
	: EditorTypePropertyBase(propertyName), dataType(inDataType), assetName(inAssetName), assetPropertyName(inAssetPropertyName) {}

	//~ Begin EditorTypePropertyBase Interface
	void DrawImGUI() override;
	EditorTypePropertyBase* DeepCopy() override;
	bool CanReadFromFile(std::ifstream& file) const override;
	void ReadFromFile(std::ifstream& file) override;
	void WriteToFile(std::ofstream& file) override;
	void ForceSetValue(const ForceSetValueParams& params) override;
	//~ End EditorTypePropertyBase Interface

	std::string GetValue() const;

protected:

	std::string GetValueString(std::string inAssetName, std::string inAssetPropertyName) const;

	bool IsValidPropertyChoice(EditorTypePropertyBase* pAsset) const;
	
	std::string dataType; // data type this property represents, e.g. "float" for DataCompositeProperty<float>
	std::string assetName; // name of the EditorAssetDataComposite
	std::string assetPropertyName; // name of the property in said asset
};

