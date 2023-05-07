#include "pch.h"

#include "EditorAssetClass.h"

#include "EditorTypeBase.h"

EditorAssetClass::EditorAssetClass(const std::string& assetName, EditorTypeBase* pEditorType) : EditorAssetBase(assetName)
{
	pEditorTypeInstance = pEditorType->DeepCopy();
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

std::string EditorAssetClass::GetLabel()
{
	return "Class";
}
