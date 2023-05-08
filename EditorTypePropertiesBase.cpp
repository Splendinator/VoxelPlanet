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

void EditorTypePropertiesBase::ReadFromFile(std::ifstream& file)
{
	for (EditorTypePropertyBase* pProperty : pProperties)
	{
		pProperty->ReadFromFile(file);
	}
}

void EditorTypePropertiesBase::WriteToFile(std::ofstream& file)
{
	for (EditorTypePropertyBase* pProperty : pProperties)
	{
		pProperty->WriteToFile(file);
	}
}

void EditorTypePropertiesBase::OnPropertiesPopulated()
{
	for (EditorTypePropertyBase* pProperty : pProperties)
	{
		pProperty->onPropertyChanged.Add(onPropertyChangedDelegate);
	}
}

void EditorTypePropertiesBase::DeepCopyProperties(EditorTypePropertiesBase* pOther)
{
	for (EditorTypePropertyBase* pProperty : pProperties)
	{
		pOther->pProperties.push_back(pProperty->DeepCopy());
	}
	pOther->OnPropertiesPopulated();
}

void EditorTypePropertiesBase::OnPropertyChanged(const OnPropertyChangedParams& params)
{
	onPropertyChanged.Invoke(params);
}
