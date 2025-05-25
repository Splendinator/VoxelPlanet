#pragma once

#include "EditorTypePropertyBase.h"

/** EditorTypePropertyInstancedAssetPtr
*
* This class is a instanced asset ptr property (InstancedAssetPtr<TClass> pAsset)
* See InstancedAssetPtr
*/
class EditorTypePropertyInstancedAssetPtr : public EditorTypePropertyBase
{
public:
	EditorTypePropertyInstancedAssetPtr() {}
	EditorTypePropertyInstancedAssetPtr(const std::string& propertyName, const std::string& inClassName, const std::string& inAssetName) : EditorTypePropertyBase(propertyName), className(inClassName), assetName(inAssetName) {}

	//~ Begin EditorTypePropertyBase Interface
	void DrawImGUI() override;
	EditorTypePropertyBase* DeepCopy() override;
	bool CanReadFromFile(std::ifstream& file) const override;
	void ReadFromFile(std::ifstream& file) override;
	void WriteToFile(std::ofstream& file) override;
	void ForceSetValue(const ForceSetValueParams& params) override;
	//~ End EditorTypePropertyBase Interface

	const std::string& GetValue() const;

protected:
	std::string className; // name of the class of the asset
	std::string assetName; // name of the actual asset
};

