#include "pch.h"

#include "EditorTypePropertiesBase.h"

#include "EditorTypePropertyBase.h"

EditorTypePropertiesBase::~EditorTypePropertiesBase()
{
	for (EditorTypePropertyBase* pProperty : pProperties)
	{
		delete pProperty;
	}
	pProperties.clear();
}

void EditorTypePropertiesBase::DrawImGUI()
{
	ImGui::Indent();
	for (EditorTypePropertyBase* pProperty : pProperties)
	{
		pProperty->DrawImGUI();
	}
	ImGui::Unindent();
}

void EditorTypePropertiesBase::DeepCopyProperties(EditorTypePropertiesBase* pOther)
{
	for (EditorTypePropertyBase* pProperty : pProperties)
	{
		pOther->pProperties.push_back(pProperty->DeepCopy());
	}
}
