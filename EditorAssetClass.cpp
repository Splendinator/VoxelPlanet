#include "pch.h"

#include "EditorAssetClass.h"

#include "EditorTypeClass.h"

EditorAssetClass::EditorAssetClass(const std::string& assetName, EditorTypeClass* pEditorType, const std::filesystem::path& assetFilePath) : EditorAssetBase(assetName, assetFilePath)
{
	pEditorTypeInstance = static_cast<EditorTypeClass *>(pEditorType->DeepCopy());
	pEditorTypeInstance->onPropertyChanged.Add(onPropertyChangedDelegate);
}

EditorAssetClass::~EditorAssetClass()
{
	delete pEditorTypeInstance;
}

void EditorAssetClass::Draw()
{
	ImGui::Text("%s", name.c_str());

	if (pEditorTypeInstance)
	{
		pEditorTypeInstance->DrawImGUI();
	}
}

void EditorAssetClass::ReadFromFile(std::ifstream& file)
{
	std::string unused;
	file >> unused >> name;

	if (pEditorTypeInstance)
	{
		pEditorTypeInstance->ReadFromFile(file);
	}
}

void EditorAssetClass::WriteToFile(std::ofstream& file)
{
	file << GetLabel() << " " << name << std::endl;

	if (pEditorTypeInstance)
	{
		pEditorTypeInstance->WriteToFile(file);
	}
}

EditorTypeBase* EditorAssetClass::GetEditorType() const
{
	return pEditorTypeInstance;
}

const std::vector<EditorTypePropertyBase*>& EditorAssetClass::GetProperties()
{
	DOMASSERT(pEditorTypeInstance, "Trying to get properties when none exist yet");
	return pEditorTypeInstance->pProperties;
}

std::string EditorAssetClass::GetLabel()
{
	return "Object";
}
