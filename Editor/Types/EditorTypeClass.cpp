#include "pch.h"

#include "EditorTypeClass.h"

void EditorTypeClass::DrawImGUI()
{
	if (ImGui::TreeNodeEx(name.c_str(), ImGuiTreeNodeFlags_DefaultOpen))
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

void EditorTypeClass::ReadFromFile(std::ifstream& file)
{
	std::string unused;
	file >> unused >> name;
	EditorTypePropertiesBase::ReadFromFile(file);
}

void EditorTypeClass::WriteToFile(std::ofstream& file)
{
	file << "Class " << name << std::endl;
	EditorTypePropertiesBase::WriteToFile(file);
}
