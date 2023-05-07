#include "pch.h"

#include "EditorTypeClass.h"

void EditorTypeClass::DrawImGUI()
{
	if (ImGui::TreeNode(name.c_str()))
	{
		EditorTypePropertiesBase::DrawImGUI();
		ImGui::TreePop();
	}
}

EditorTypeBase* EditorTypeClass::DeepCopy()
{
	EditorTypeClass* pCopy = new EditorTypeClass();
	pCopy->name = name;
	DeepCopyProperties(pCopy);
	return pCopy;
}
