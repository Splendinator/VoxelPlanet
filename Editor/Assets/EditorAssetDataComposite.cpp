#include "pch.h"

#include "EditorAssetDataComposite.h"

EditorAssetDataComposite::EditorAssetDataComposite(const std::string& assetName, const std::filesystem::path& inAssetfilePath): EditorAssetBase(assetName, inAssetfilePath)
{
	editorTypeDataComposite.onPropertyChanged.Add(onPropertyChangedDelegate);
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
