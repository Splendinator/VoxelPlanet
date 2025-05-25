#include "pch.h"

#include "EditorWindowInsertDataCompositeItem.h"

#include "Editor/Types/EditorTypeEnum.h"
#include "Editor/Types/EditorTypeStruct.h"
#include "Editor/Types/Properties/EditorTypePropertyBool.h"
#include "Editor/Types/Properties/EditorTypePropertyClass.h"
#include "Editor/Types/Properties/EditorTypePropertyEnum.h"
#include "Editor/Types/Properties/EditorTypePropertyFloat.h"
#include "Editor/Types/Properties/EditorTypePropertyInt.h"
#include "Editor/Types/Properties/EditorTypePropertyString.h"
#include "Editor/Types/Properties/EditorTypePropertyStruct.h"
#include "Editor/Types/Properties/EditorTypePropertyVector.h"

std::string EditorWindowInsertDataCompositeItem::GetWindowName() const
{
	return "Insert Data Composite Item";
}

void EditorWindowInsertDataCompositeItem::Open()
{
	std::vector<std::string> allPropertyTypes;

	allPropertyTypes.emplace_back("float");
	allPropertyTypes.emplace_back("int");
	allPropertyTypes.emplace_back("bool");
	allPropertyTypes.emplace_back("string");
	
	std::vector<std::string> allClasses = Game::GetAssetManager().GetAllClassTemplateNames(false);
	allPropertyTypes.insert(allPropertyTypes.end(), allClasses.begin(), allClasses.end());
	
	std::vector<std::string> allStructs = Game::GetAssetManager().GetAllStructTemplateNames(false);
	allPropertyTypes.insert(allPropertyTypes.end(), allStructs.begin(), allStructs.end());
	
	std::vector<std::string> allEnums = Game::GetAssetManager().GetAllEnumTypeNames();
	allPropertyTypes.insert(allPropertyTypes.end(), allEnums.begin(), allEnums.end());

	propertyTypeList.SetItems(allPropertyTypes);
}

void EditorWindowInsertDataCompositeItem::Draw()
{
	propertyTypeList.Draw();

	std::string selectedItem = propertyTypeList.GetSelectedItem();

	ImGui::InputText("Property Name", propertyNameBuffer, sizeof(propertyNameBuffer));

	// #TODO: We don't currently support vectors, see #JANK comment in EditorTypeDataComposite::ReadFromFile()
	//ImGui::Checkbox("Vector", &bCreateVector);

	// Validation
	{
		ImVec4 redColour(1.0f, 0.0f, 0.0f, 1.0f);
		ImGui::PushStyleColor(ImGuiCol_Text, redColour);
		if (selectedItem.empty())
		{
			ImGui::Text("No property type selected");
			ImGui::PopStyleColor();
			return;
		}

		if (propertyNameBuffer[0] == '\0')
		{
			ImGui::Text("Empty property name");
			ImGui::PopStyleColor();
			return;
		}
		
		ImGui::PopStyleColor();
	}

	if (ImGui::Button("Add Property"))
	{
		EditorTypePropertyBase* pNewProperty = nullptr;
		if (selectedItem == "float")
		{
			pNewProperty = new EditorTypePropertyFloat(bCreateVector ? "float" : propertyNameBuffer, 0.0f);
		}
		else if (selectedItem == "int")
		{
			pNewProperty = new EditorTypePropertyInt(bCreateVector ? "int" : propertyNameBuffer, 0);
		}
		else if (selectedItem == "bool")
		{
			pNewProperty = new EditorTypePropertyBool(bCreateVector ? "bool" : propertyNameBuffer, false);
		}
		else if (selectedItem == "string")
		{
			pNewProperty = new EditorTypePropertyString(bCreateVector ? "string" : propertyNameBuffer, "");
		}
		else if (EditorTypeBase* pClassType = Game::GetAssetManager().FindClassTemplateType(selectedItem))
		{
			pNewProperty = new EditorTypePropertyClass(bCreateVector ? pClassType->name : propertyNameBuffer, pClassType->name, "nullptr");
		}
		else if (EditorTypeStruct* pStructType = Game::GetAssetManager().FindStructTemplateType(selectedItem))
		{
			pNewProperty = new EditorTypePropertyStruct(bCreateVector ? pStructType->name : propertyNameBuffer, pStructType->name, static_cast<EditorTypeStruct*>(pStructType->DeepCopy()));
		}
		else if (EditorTypeBase* pEnumType = Game::GetAssetManager().FindEnumType(selectedItem))
		{
			pNewProperty = new EditorTypePropertyEnum(bCreateVector ? pEnumType->name : propertyNameBuffer, pEnumType->name, 0);
		}
		else
		{
			DOMLOG_ERROR("Unknown property type", selectedItem);
			return;
		}

		if (bCreateVector)
		{
			EditorTypePropertyVector* pNewVectorProperty = new EditorTypePropertyVector(pNewProperty, propertyNameBuffer);
			pNewProperty = pNewVectorProperty;
		}
		
		dataComposite.AddAndSetupProperty(pNewProperty, insertIndex);
		
		RequestClose();
	}
}

