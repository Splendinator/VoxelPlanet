#include "pch.h"

#include "EditorAssetDataComposite.h"

#include "Editor/Types/Properties/EditorTypePropertyBase.h"

EditorAssetDataComposite::EditorAssetDataComposite(const std::string& assetName, const std::filesystem::path& inAssetfilePath): EditorAssetBase(assetName, inAssetfilePath)
{
	editorTypeDataComposite.onPropertyChanged.Add(onPropertyChangedDelegate);
}

std::vector<std::string> EditorAssetDataComposite::GetPropertyNames() const
{
	std::vector<std::string> propertyNames;
	
	const std::vector<EditorTypePropertyBase*>& properties = editorTypeDataComposite.GetProperties();

	for (EditorTypePropertyBase* pProperty : properties)
	{
		propertyNames.push_back(pProperty->GetName());
	}

	return propertyNames;
}

EditorTypePropertyBase* EditorAssetDataComposite::FindProperty(std::string propertyName) const
{
	const std::vector<EditorTypePropertyBase*>& properties = editorTypeDataComposite.GetProperties();
	for (EditorTypePropertyBase* pProperty : properties)
	{
		if (pProperty->GetName() == propertyName)
		{
			return pProperty;
		}
	}

	return nullptr;
}

void EditorAssetDataComposite::Draw()
{
	editorTypeDataComposite.DrawImGUI();
}

void EditorAssetDataComposite::ReadFromFile(std::ifstream& file)
{
	std::string temp;
	file >> temp;
	DOMLOG_ERROR_IF(temp != "DataComposite", "Expected DataComposite keyword, got:", temp);

	file >> name;

	editorTypeDataComposite.ReadFromFile(file);
}

void EditorAssetDataComposite::WriteToFile(std::ofstream& file)
{
	file << "DataComposite " << name << std::endl;

	editorTypeDataComposite.WriteToFile(file);
}
