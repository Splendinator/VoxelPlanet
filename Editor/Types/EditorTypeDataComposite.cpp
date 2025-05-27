#include "pch.h"

#include "EditorTypeDataComposite.h"

#include "Core/GameInstance.h"
#include "Editor/Actions/EditorActionRemoveCompositeData.h"
#include "Editor/ImGuiEditor.h"
#include "Editor/Windows/EditorWindowInsertDataCompositeItem.h"
#include "EditorTypeUtils.h"
#include "Properties/EditorTypePropertyBase.h"
#include "Properties/EditorTypePropertyFactoryBase.h"
#include "Properties/EditorTypePropertyStruct.h"

void EditorTypeDataComposite::DrawImGUI()
{
	// #TODO: Removing and Adding properties should be done using EditorTypePropertiesBase::onPropertyChanged or a new EditorActionBase.
	// #TODO: We should add a new EditorActionBase that keeps the property in memory, instead of just deleting it. Right now we crash if we delete it while it's still in the action queue.
	
	if (ImGui::BeginTable("DataComposite", 2, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_SizingFixedFit))
	{
		ImGui::TableSetupColumn("Action", ImGuiTableColumnFlags_WidthFixed, 100.0f);
		ImGui::TableSetupColumn("Property", ImGuiTableColumnFlags_WidthStretch);

		for (int propertyIndex = 0; propertyIndex < pProperties.size(); ++propertyIndex)
		{
			EditorTypePropertyBase* pProperty = pProperties[propertyIndex];
			
			ImGui::PushID(pProperty);
			ImGui::TableNextColumn();
			
			if (ImGui::Button("Insert Above"))
			{
				if (ImGuiEditor* pEditor = Game::GetGameInstance().FindGameSystemSlow<ImGuiEditor>())
				{
					pEditor->AddWindow(std::make_shared<EditorWindowInsertDataCompositeItem>(pOwningAsset, propertyIndex));
				}
			}
			
			if (ImGui::Button("Remove"))
			{
				if (ImGuiEditor* pEditor = Game::GetGameInstance().FindGameSystemSlow<ImGuiEditor>())
				{
					pEditor->DoAction(std::make_shared<EditorActionRemoveCompositeData>(pOwningAsset, pProperty, propertyIndex));
				}
				ImGui::EndTable();
				ImGui::PopID();
				return;
			}

			ImGui::TableNextColumn();

			pProperty->DrawImGUI();

			ImGui::TableNextRow();
			ImGui::PopID();
		}
		ImGui::EndTable();
	}

	if (ImGui::Button("Insert Composite Data Item"))
	{
		if (ImGuiEditor* pEditor = Game::GetGameInstance().FindGameSystemSlow<ImGuiEditor>())
		{
			pEditor->AddWindow(std::make_shared<EditorWindowInsertDataCompositeItem>(pOwningAsset));
		}
	}
}

EditorTypeBase* EditorTypeDataComposite::DeepCopy()
{
    DOMLOG_ERROR("I can't think of why we would want to DeepCopy a DataComposite. Is this a mistake?");
    return nullptr;
}

void EditorTypeDataComposite::ReadFromFile(std::ifstream& file)
{
	while (true)
	{
		std::streampos preKeywordStreamPosition = file.tellg();
		std::string keyword;
		file >> keyword;
		if (keyword == ";")
		{
			OnPropertiesPopulated();
			return;
		}

		file.seekg(preKeywordStreamPosition); // We want to pass the keyword to ReadFromFile() to keep everything consistent.

		// This needs explaining. Essentially the EditorTypePropertyFactoryBase were designed to read in the initial template types from EditorTypes.txt, which are then deep copied whenever we create a new asset of that type.
		// It just so happens that the way *most* of them are implemented, they can also be used to read in properties from a DataComposite file, due to similar file structure and code.
		// (see EditorTypePropertyFactoryClass::CreateType(), it literally just forwards to EditorTypePropertyClass::ReadFromFile())
		// There are certain exceptions like structs so we handle those separately underneath.
		// #JANK: We really shouldn't be re-using these factories for reading in DataComposite files, but it works for now and is a lot less code than writing a new set of factories just for this purpose.
		// #JANK: I didn't implement vector types here because it's a lot of code (The solution would be similar to EditorTypePropertyFactoryVector::CreateType()). We can work around this by using a struct with a vector property inside it.
		static EditorTypePropertyFactoryBase* propertyFactories[] = 
		{
			&EditorTypeUtils::floatFactory,
			&EditorTypeUtils::boolFactory,
			&EditorTypeUtils::intFactory,
			&EditorTypeUtils::stringFactory,
			&EditorTypeUtils::classFactory,
			&EditorTypeUtils::vectorFactory,
			&EditorTypeUtils::enumFactory,
			&EditorTypeUtils::instancedAssetFactory,
		};

		bool bPropertyFound = false;
		for (EditorTypePropertyFactoryBase* pFactory : propertyFactories)
		{
			if (pFactory->GetLabel() == keyword)
			{
				bPropertyFound = true;
				
				EditorTypePropertyBase* pProperty = pFactory->CreateType(file);
				pProperties.push_back(pProperty);
				break;
			}
		}

		if (!bPropertyFound)
		{
			if (keyword == "struct")
			{
				EditorTypePropertyStruct* pProperty = new EditorTypePropertyStruct();
				pProperty->ReadFromFile(file);
				pProperties.push_back(pProperty);
			}
			else
			{
				DOMLOG_ERROR("Unknown property type in DataComposite file:", keyword);
			}
		}
	}
}

void EditorTypeDataComposite::WriteToFile(std::ofstream& file)
{
    EditorTypePropertiesBase::WriteToFile(file);
}