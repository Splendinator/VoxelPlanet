#include "pch.h"
#include "EditorTypePropertyVector.h"

EditorTypePropertyVector::EditorTypePropertyVector(EditorTypePropertyBase* templateType, const std::string& inName)
: dataTemplateType(templateType), EditorTypePropertyBase(inName)
{
	onInternalVectorPropertyChanged.Bind(this, &EditorTypePropertyVector::OnInternalVectorPropertyChanged);
}

void EditorTypePropertyVector::DrawImGUI()
{
	const std::string& imGuiTreeLabel = name;
	if (ImGui::TreeNodeEx(imGuiTreeLabel.c_str(), ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::SameLine();
		if (ImGui::Button("Add"))
		{
			AddDefaultEntry();
		}
		
		for (int index = 0; index < arrayLength; ++index)
		{
			const std::string& arrayEntryLabel = std::to_string(index);
			if (ImGui::TreeNodeEx(arrayEntryLabel.c_str(), ImGuiTreeNodeFlags_DefaultOpen))
			{
				// #TEMP: I can't fucking write shit in this textbox, it loses focus
				instancedProperties[index]->DrawImGUI();
				ImGui::TreePop();
			}
		}
		
		ImGui::TreePop();
	}
}

void EditorTypePropertyVector::ReadFromFile(std::ifstream& file)
{
	std::string temp;
	file >> temp; // "vector"
	file >> name; // "myVector"
	file >> temp; // "6"

	const int numEntries = std::stoi(temp);
	for (int i = 0; i < numEntries; ++i)
	{
		AddDefaultEntry();
	}

	for (std::unique_ptr<EditorTypePropertyBase>& instancedProperty : instancedProperties)
	{
		instancedProperty->ReadFromFile(file);
	}
}

void EditorTypePropertyVector::WriteToFile(std::ofstream& file)
{
	file << "vector " << name << " " << std::to_string(arrayLength) << std::endl;
	
	for (std::unique_ptr<EditorTypePropertyBase>& instancedProperty : instancedProperties)
	{
		instancedProperty->WriteToFile(file);
	}
}

EditorTypePropertyBase* EditorTypePropertyVector::DeepCopy()
{
	EditorTypePropertyVector* deepCopiedVector = new EditorTypePropertyVector(dataTemplateType->DeepCopy(), name);
	for (std::unique_ptr<EditorTypePropertyBase>& instancedProperty : instancedProperties)
	{
		deepCopiedVector->AddEntry(instancedProperty->DeepCopy());
	}
	return deepCopiedVector;
}

void EditorTypePropertyVector::ForceSetValue(const std::string& newValue)
{
	// Unused for vectors, individual properties will use this
}

void EditorTypePropertyVector::OnInternalVectorPropertyChanged(const OnPropertyChangedParams& onPropertyChangedParams)
{
	onPropertyChanged.Invoke(onPropertyChangedParams);
}

void EditorTypePropertyVector::AddEntry(EditorTypePropertyBase* pEntry)
{
	++arrayLength;
	pEntry->onPropertyChanged.Add(onInternalVectorPropertyChanged);
	instancedProperties.push_back(std::unique_ptr<EditorTypePropertyBase>(pEntry));
}

void EditorTypePropertyVector::AddDefaultEntry()
{
	AddEntry(dataTemplateType->DeepCopy());
}
