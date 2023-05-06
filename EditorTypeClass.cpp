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
