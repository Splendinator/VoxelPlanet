#include "pch.h"

#include "EditorTypePropertyStruct.h"

#include "EditorTypeStruct.h"
#include "WritePropertyToFileUtils.h"
#include "__Generated.h"

EditorTypePropertyStruct::EditorTypePropertyStruct(const std::string& propertyName, const std::string& inStructName, EditorTypeStruct* pInStructType) :
EditorTypePropertyBase(propertyName), structName(inStructName), pStructType(pInStructType)
{
	DOMASSERT(pInStructType);

	onInternalStructPropertyChangedDelegate.Bind(this, &EditorTypePropertyStruct::OnInternalStructPropertyChanged);
	pStructType->onPropertyChanged.Add(onInternalStructPropertyChangedDelegate); // #TODO: Should remove on destruct but the pStructType will always be unique to this class.
}

void EditorTypePropertyStruct::DrawImGUI()
{
	std::string imGuiTreeLabel = name + " (" + structName + ")";
	
	if (ImGui::TreeNodeEx(imGuiTreeLabel.c_str(), ImGuiTreeNodeFlags_DefaultOpen))
	{
		pStructType->DrawImGUI();
		ImGui::TreePop();
	}
}

EditorTypePropertyBase* EditorTypePropertyStruct::DeepCopy()
{
	return new EditorTypePropertyStruct(name, structName, static_cast<EditorTypeStruct*>(pStructType->DeepCopy()));
}

bool EditorTypePropertyStruct::CanReadFromFile(std::ifstream& file) const
{
	std::string temp;

	file >> temp;
	if (temp != "struct")
	{
		return false;
	}

	file >> temp;
	if (temp != structName)
	{
		return false;
	}

	file >> temp;
	return temp == name;
}

void EditorTypePropertyStruct::ReadFromFile(std::ifstream& file)
{
	std::string unused;
	file >> unused >> structName >> name; // "Struct MyStruct myStruct"

	pStructType->ReadFromFile(file);
}

void EditorTypePropertyStruct::WriteToFile(std::ofstream& file)
{
	PropertyFileUtils::WriteStructToFile(file, name, structName);
	pStructType->WriteToFile(file);
}

void EditorTypePropertyStruct::ForceSetValue(const std::string& newValue)
{
	// Unused for structs, individual properties will use this
}

void* EditorTypePropertyStruct::GetValue() const
{
	auto it = __Generated::stringToCreateObjectFunction.find(pStructType->name);
	if (it != __Generated::stringToCreateObjectFunction.end())
	{
		return it->second(pStructType->pProperties);
	}
	
	DOMLOG_ERROR("Struct", name, "not found");
	return nullptr;
}

void EditorTypePropertyStruct::OnInternalStructPropertyChanged(const OnPropertyChangedParams& onPropertyChangedParams)
{
	onPropertyChanged.Invoke(onPropertyChangedParams);
}
