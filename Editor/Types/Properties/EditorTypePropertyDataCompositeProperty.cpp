#include "pch.h"

#include "EditorTypePropertyDataCompositeProperty.h"

#include "WritePropertyToFileUtils.h"
#include "Editor/Assets/EditorAssetDataComposite.h"


void EditorTypePropertyDataCompositeProperty::DrawImGUI()
{
	std::string imGuiTreeLabel = name + " (DataCompositeProperty)";
	
	if (ImGui::TreeNodeEx(imGuiTreeLabel.c_str(), ImGuiTreeNodeFlags_DefaultOpen))
	{
		// Drop down for asset
		if (ImGui::BeginCombo("pDataCompositeAsset", assetName.c_str()))
		{
			std::vector<std::weak_ptr<EditorAssetDataComposite>> validAssets = Game::GetAssetManager().GatherAssetsOfType<EditorAssetDataComposite>();
		
			auto RenderSelectable = [this](const std::string& selectableAssetName)
			{
				bool bIsSelected = selectableAssetName == assetName;
				if (ImGui::Selectable(selectableAssetName.c_str(), &bIsSelected))
				{
					onPropertyChanged.Invoke({this, GetValueString(assetName, assetPropertyName), GetValueString(selectableAssetName, "none")});
				}
			};
		
			RenderSelectable("nullptr");
			for (auto validAsset : validAssets)
			{
				std::string selectableAssetName = validAsset.lock().get()->GetName();
				RenderSelectable(selectableAssetName);
			}
		
			ImGui::EndCombo();
		}

		// Drop down for property
		if (ImGui::BeginCombo("propertyName", assetPropertyName.c_str()))
		{
			auto RenderSelectable = [this](const std::string& selectablePropertyName)
			{
				bool bIsSelected = selectablePropertyName == assetName;
				if (ImGui::Selectable(selectablePropertyName.c_str(), &bIsSelected))
				{
					onPropertyChanged.Invoke({this, GetValueString(assetName, assetPropertyName), GetValueString(assetName, selectablePropertyName)});
				}
			};

			RenderSelectable("none");

			if (assetName != "nullptr")
			{
				std::weak_ptr<EditorAssetDataComposite> pFoundAsset = Game::GetAssetManager().FindAsset<EditorAssetDataComposite>(assetName);

				if (EditorAssetDataComposite* pEditorAssetDataComposite = pFoundAsset.lock().get())
				{
					for (std::string propertyName : pEditorAssetDataComposite->GetPropertyNames())
					{
						if (IsValidPropertyChoice(pEditorAssetDataComposite->FindProperty(propertyName)))
						{
							RenderSelectable(propertyName);
						}
					}
				}
			}
		
			ImGui::EndCombo();
		}
		
		ImGui::TreePop();
	}
}

EditorTypePropertyBase* EditorTypePropertyDataCompositeProperty::DeepCopy()
{
	return new EditorTypePropertyDataCompositeProperty(name, dataType, assetName, assetPropertyName);
}

bool EditorTypePropertyDataCompositeProperty::CanReadFromFile(std::ifstream& file) const
{
	std::string temp;

	file >> temp;
	if (temp != "datacompositeproperty")
	{
		return false;
	}
	
	file >> temp;
	if (temp != name)
	{
		return false;
	}

	return true;
}

void EditorTypePropertyDataCompositeProperty::ReadFromFile(std::ifstream& file)
{
	// Ready in "datacompositeproperty myVariable float assetName assetPropertyName"
	std::string unused;
	file >> unused >> name >> dataType >> assetName >> assetPropertyName;
}

void EditorTypePropertyDataCompositeProperty::WriteToFile(std::ofstream& file)
{
	PropertyFileUtils::WriteDataCompositePropertyToFile(file, name, dataType, assetName, assetPropertyName);
}

void EditorTypePropertyDataCompositeProperty::ForceSetValue(const ForceSetValueParams& params)
{
	std::string value = params.newValue;
	size_t pos = value.find("::");

	DOMLOG_ERROR_IF(pos == std::string::npos, "Expected '::' in value for EditorTypePropertyDataCompositeProperty", value);
	
	assetName = value.substr(0, pos);
	assetPropertyName = value.substr(pos + 2); // Skip over the "::"
}

std::string EditorTypePropertyDataCompositeProperty::GetValue() const
{
	return GetValueString(assetName, assetPropertyName);
}

std::string EditorTypePropertyDataCompositeProperty::GetValueString(std::string inAssetName, std::string inAssetPropertyName) const
{
	return inAssetName + "::" + inAssetPropertyName;
}

bool EditorTypePropertyDataCompositeProperty::IsValidPropertyChoice(EditorTypePropertyBase* pAsset) const
{
	if (pAsset == nullptr)
	{
		return false;
	}
	
	if (dataType == "float" && dynamic_cast<EditorTypePropertyFloat*>(pAsset))
	{
		return true;
	}
	if (dataType == "int" && dynamic_cast<EditorTypePropertyInt*>(pAsset))
	{
		return true;
	}
	if (dataType == "bool" && dynamic_cast<EditorTypePropertyBool*>(pAsset))
	{
		return true;
	}
	if (dataType == "std::string" && dynamic_cast<EditorTypePropertyString*>(pAsset))
	{
		return true;
	}

	if (EditorTypePropertyEnum* pEnumProperty = dynamic_cast<EditorTypePropertyEnum*>(pAsset))
	{
		return pEnumProperty->GetEnumName() == dataType;
	}
	if (EditorTypePropertyClass* pClassProperty = dynamic_cast<EditorTypePropertyClass*>(pAsset))
	{
		return pClassProperty->GetClassName() == dataType;
	}
	if (EditorTypePropertyStruct* pStructProperty = dynamic_cast<EditorTypePropertyStruct*>(pAsset))
	{
		return pStructProperty->GetStructName() == dataType;
	}

	return false;
}
