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
	if (ImGui::TreeNodeEx(imGuiTreeLabel.c_str()))
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
				instancedProperties[index]->DrawImGUI();
				ImGui::TreePop();
			}
			ImGui::SameLine();
			{
				ImGui::PushID(index);
				if (ImGui::Button("Remove"))
				{
					if (arrayLength - 1 != index)
					{
						std::swap(instancedProperties[arrayLength-1], instancedProperties[index]);
					}

					OnPropertyChangedParams arrayLengthChangedParams = {};
					arrayLengthChangedParams.pProperty = this;
					arrayLengthChangedParams.oldValue = std::to_string(arrayLength);

					--arrayLength;
				
					arrayLengthChangedParams.newValue = std::to_string(arrayLength);

					onPropertyChanged.Invoke(arrayLengthChangedParams);
				}
				ImGui::PopID();
			}
		}
		
		ImGui::TreePop();
	}
}

bool EditorTypePropertyVector::CanReadFromFile(std::ifstream& file) const
{
	std::string temp;

	file >> temp;
	if (temp != "vector")
	{
		return false;
	}

	file >> temp;
	return temp == name;
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

	// We use arrayLength here and not a foreach loop as it can differ from the length of the array in the case of undoing a delete etc.
	// This is by design so that if you re-do it gets back the value that was in there.
	for (int i = 0; i < arrayLength; ++i)
	{
		std::unique_ptr<EditorTypePropertyBase>& instancedProperty = instancedProperties[i];
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
	// When a vector gets ForceSetValue called it's getting in a array length.
	// It's a bit janky maybe to re-use the property changing system for array resizing but, honestly,
	// I'm not even sure it is that janky. I think it's literally fine (apart from the text in the action log looks fucky)
	
	const int newArrayLength = std::stoi(newValue);

	arrayLength = newArrayLength;
}

void EditorTypePropertyVector::OnInternalVectorPropertyChanged(const OnPropertyChangedParams& onPropertyChangedParams)
{
	onPropertyChanged.Invoke(onPropertyChangedParams);
}

void EditorTypePropertyVector::AddEntry(EditorTypePropertyBase* pEntry)
{
	++arrayLength;
	pEntry->onPropertyChanged.Add(onInternalVectorPropertyChanged);

	if (arrayLength <= instancedProperties.size()) 
	{
		// this can happen in the case of adding a new entry after undoing the last one.
		instancedProperties[arrayLength-1].release();
		instancedProperties[arrayLength-1].reset(pEntry);
	}
	else
	{
		instancedProperties.push_back(std::unique_ptr<EditorTypePropertyBase>(pEntry));
	}
}

void EditorTypePropertyVector::AddDefaultEntry()
{
	OnPropertyChangedParams arrayLengthChangedParams = {};
	arrayLengthChangedParams.pProperty = this;
	arrayLengthChangedParams.oldValue = std::to_string(arrayLength);

	AddEntry(dataTemplateType->DeepCopy());

	arrayLengthChangedParams.newValue = std::to_string(arrayLength);
	
	onPropertyChanged.Invoke(arrayLengthChangedParams);
}
