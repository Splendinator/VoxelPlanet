#include "pch.h"

#include "EditorTypePropertyClass.h"

#include "AssetManager/AssetManager.h"
#include "EditorAssetBase.h"
#include "Game.h"
#include "WritePropertyToFileUtils.h"
#include "EditorTypeClass.h"
#include "__Generated.h"

EditorTypePropertyClass::EditorTypePropertyClass()
{
	onEditInlineNewClassPropertyChangedDelegate.Bind(this, &EditorTypePropertyClass::OnEditInlineNewClassPropertyChanged);
}

EditorTypePropertyClass::EditorTypePropertyClass(const std::string& propertyName, const std::string& inClassName, const std::string& inAssetName) :
EditorTypePropertyBase(propertyName), className(inClassName), assetName(inAssetName)
{
	onEditInlineNewClassPropertyChangedDelegate.Bind(this, &EditorTypePropertyClass::OnEditInlineNewClassPropertyChanged);
}

void EditorTypePropertyClass::DrawImGUI()
{
	EditorTypeClass* pClassTemplate = Game::GetAssetManager().FindClassTemplateType(className);
	if (pClassTemplate->HasMetadataFlag(EClassMetadataFlags::EditInlineNew))
	{
		if (pEditInlineNewClass)
		{
			ImGui::Text(name.c_str());
			ImGui::SameLine();
			if (ImGui::Button("Clear EditInlineNew"))
			{
				OnPropertyChangedData params = {this, assetName, "nullptr"};
				params.oldClassValue = pEditInlineNewClass;
				
				onPropertyChanged.Invoke(params);
				
				return;
			}
			pEditInlineNewClass->DrawImGUI();
		}
		else
		{
			if (ImGui::BeginCombo(name.c_str(), "nullptr"))
			{
				std::vector<std::string> childClassTemplates = Game::GetAssetManager().GetAllChildClassTemplateNames(className, true);

				for (const std::string& childClassTemplate : childClassTemplates)
				{
					bool bIsSelected = false;
					if (ImGui::Selectable(childClassTemplate.c_str(), &bIsSelected))
					{
						EditorTypeClass* pSelectedClassType = Game::GetAssetManager().FindClassTemplateType(childClassTemplate);
						
						OnPropertyChangedData params = {this, assetName, childClassTemplate};
						params.newClassValue.reset(static_cast<EditorTypeClass*>(pSelectedClassType->DeepCopy()));

						onPropertyChanged.Invoke(params);

						pEditInlineNewClass->onPropertyChanged.Add(onEditInlineNewClassPropertyChangedDelegate);
					}
				}

				ImGui::EndCombo();
			}	
		}
	}
	else
	{
		if (ImGui::BeginCombo(name.c_str(), assetName.c_str()))
		{
			std::vector<std::weak_ptr<EditorAssetBase>> validAssets = Game::GetAssetManager().GatherAssetsOfClass(className, true);
	
			auto RenderSelectable = [this](const std::string& selectableAssetName)
			{
				bool bIsSelected = selectableAssetName == assetName;
				if (ImGui::Selectable(selectableAssetName.c_str(), &bIsSelected))
				{
					onPropertyChanged.Invoke({this, assetName, selectableAssetName});
				}
			};
	
			RenderSelectable("nullptr");
			for (auto validAsset : validAssets)
			{
				std::string selectableAssetName = validAsset.lock().get()->GetName().c_str();
				RenderSelectable(selectableAssetName);
			}
	
			ImGui::EndCombo();
		}
	}
}

EditorTypePropertyBase* EditorTypePropertyClass::DeepCopy()
{
	EditorTypePropertyClass* pNewClass = new EditorTypePropertyClass(name, className, assetName);

	if (pEditInlineNewClass)
	{
		pNewClass->SetEditInlineNewClass(static_cast<EditorTypeClass*>(pEditInlineNewClass->DeepCopy()));	
	}
	
	return pNewClass;
}

bool EditorTypePropertyClass::CanReadFromFile(std::ifstream& file) const
{
	std::string temp;

	file >> temp;
	if (temp != "class")
	{
		return false;
	}

	file >> temp;
	if (temp != className)
	{
		return false;
	}

	file >> temp;
	return temp == name;
}

// #TEMP: Optimisation
#pragma optimize("", off)
void EditorTypePropertyClass::ReadFromFile(std::ifstream& file)
{
	// Read in "class SingleFloat pNext nullptr"
	
	std::string unused;
	file >> unused >> className >> name >> assetName;

	if (assetName != "nullptr")
	{
		EditorTypeClass* pClassTemplate = Game::GetAssetManager().FindClassTemplateType(className);
		if (pClassTemplate == nullptr)
		{
			DOMLOG_ERROR("Can't find template type for class", className, ". Perhaps this class was renamed? Nulling out property.")
			assetName = "nullptr";
			return;
		}
		
		if (pClassTemplate->HasMetadataFlag(EClassMetadataFlags::EditInlineNew))
		{
			EditorTypeClass* pFoundTemplateType = Game::GetAssetManager().FindClassTemplateType(assetName);

			if (pFoundTemplateType == nullptr)
			{
				DOMLOG_ERROR("Can't find template type for class", assetName, ". Perhaps this class was renamed? Nulling out property.")
				assetName = "nullptr";
				return;
			}
			
			SetEditInlineNewClass(static_cast<EditorTypeClass*>(pFoundTemplateType->DeepCopy()));
			pEditInlineNewClass->ReadFromFile(file);
		}
	}
}
#pragma optimize("", on)

void EditorTypePropertyClass::WriteToFile(std::ofstream& file)
{
	PropertyFileUtils::WriteClassToFile(file, name, className, assetName);
	if (pEditInlineNewClass)
	{
		pEditInlineNewClass->WriteToFile(file);
	}
}

void EditorTypePropertyClass::ForceSetValue(const ForceSetValueParams& params)
{
	assetName = params.newValue;
	pEditInlineNewClass = params.newClassValue;
}

void* EditorTypePropertyClass::GetValue() const
{
	if (pEditInlineNewClass)
	{
		auto it = __Generated::stringToCreateObjectFunction.find(pEditInlineNewClass->name);
		if (it != __Generated::stringToCreateObjectFunction.end())
		{
			return it->second(pEditInlineNewClass->pProperties);
		}

		return nullptr;
	}
	
	if (assetName != "nullptr" && assetName != "")
	{
		return Game::GetAssetManager().LoadObjectFromAssetName<void>(assetName);
	}
	return nullptr;
}

void EditorTypePropertyClass::SetEditInlineNewClass(EditorTypeClass* editInlineNewClass)
{
	pEditInlineNewClass.reset(editInlineNewClass);
	pEditInlineNewClass->onPropertyChanged.Add(onEditInlineNewClassPropertyChangedDelegate); // #TODO: Should remove on destruct but the pEditInlineNewClass will always be unique to this class.
}

void EditorTypePropertyClass::OnEditInlineNewClassPropertyChanged(const OnPropertyChangedData& OnPropertyChangedData)
{
	onPropertyChanged.Invoke(OnPropertyChangedData);
}
