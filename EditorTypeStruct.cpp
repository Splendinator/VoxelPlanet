#include "pch.h"

#include "EditorTypeStruct.h"

void EditorTypeStruct::DrawImGUI()
{
	EditorTypePropertiesBase::DrawImGUI();
}

EditorTypeBase* EditorTypeStruct::DeepCopy()
{
	EditorTypeStruct* pCopy = new EditorTypeStruct();
	pCopy->name = name;
	DeepCopyProperties(pCopy);
	return pCopy;
}

void EditorTypeStruct::ReadFromFile(std::ifstream& file)
{
	EditorTypePropertiesBase::ReadFromFile(file);
}

void EditorTypeStruct::WriteToFile(std::ofstream& file)
{
	EditorTypePropertiesBase::WriteToFile(file);
}
