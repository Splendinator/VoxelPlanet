#include "pch.h"

#include "EditorTypePropertiesBase.h"

#include "EditorTypePropertyBase.h"

void EditorTypePropertiesBase::DrawImGUI()
{
	ImGui::Indent();
	for (EditorTypePropertyBase* pProperty : pProperties)
	{
		pProperty->DrawImGUI();
	}
	ImGui::Unindent();
}
