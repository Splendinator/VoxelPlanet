#include "pch.h"

#include "EditorTypePropertiesBase.h"

#include "AssetManager/AssetManager.h"
#include "EditorTypePropertyBase.h"
#include "Game.h"

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
	for (EditorTypePropertyBase* pProperty : pProperties)
	{
		pProperty->DrawImGUI();
	}
}

void EditorTypePropertiesBase::ReadFromFile(std::ifstream& file)
{
	while (file.peek() != ';')
	{
		std::streampos preCanReadCheckPosition = file.tellg();
		for (EditorTypePropertyBase* pProperty : pProperties)
		{
			const bool bCanRead = pProperty->CanReadFromFile(file);
			file.seekg(preCanReadCheckPosition); // Go back to before the read check so we can read normally
			
			if (bCanRead)
			{
				pProperty->ReadFromFile(file);
				break;
			}
		}

		std::string temp;
		std::getline(file, temp); // Move on to next line (in the case of all properties failing this is needed)
	}

	file.get(); // Get the ';'
}

void EditorTypePropertiesBase::WriteToFile(std::ofstream& file)
{
	for (EditorTypePropertyBase* pProperty : pProperties)
	{
		pProperty->WriteToFile(file);
	}
	file << ';' << '\n';
}

void EditorTypePropertiesBase::OnPropertiesPopulated()
{
	for (EditorTypePropertyBase* pProperty : pProperties)
	{
		pProperty->onPropertyChanged.Add(onPropertyChangedDelegate);
	}
}

void EditorTypePropertiesBase::OnTemplateMetadataFlagsPopulated()
{
	// We want to inherit Singleton/Abstract from the base
	{
		const bool bInstanced = HasMetadataFlag(EClassMetadataFlags::Instanced);
		const bool bSingleton = HasMetadataFlag(EClassMetadataFlags::Singleton);
		DOMLOG_ERROR_IF(bInstanced && bSingleton, "Can't be both singleton and instanced.");
		
		if (!bInstanced && !bSingleton)
		{
			// No explicit flag set, try and derrive from child, or default to instanced
			
			const EClassMetadataFlags derrivedFlag = GetChildInstancedOrSingletonFlag();
			AddMetadataFlag(derrivedFlag == EClassMetadataFlags::None ? EClassMetadataFlags::Instanced : derrivedFlag);
		}
	}
}

void EditorTypePropertiesBase::DeepCopyProperties(EditorTypePropertiesBase* pOther)
{
	for (EditorTypePropertyBase* pProperty : pProperties)
	{
		pOther->pProperties.push_back(pProperty->DeepCopy());
	}

	pOther->metadataFlags = metadataFlags;

	pOther->OnPropertiesPopulated();
}

void EditorTypePropertiesBase::OnPropertyChanged(const OnPropertyChangedData& params)
{
	onPropertyChanged.Invoke(params);
}

EClassMetadataFlags EditorTypePropertiesBase::GetChildInstancedOrSingletonFlag()
{
	if (HasMetadataFlag(EClassMetadataFlags::Instanced))
	{
		return EClassMetadataFlags::Instanced;
	}
	if (HasMetadataFlag(EClassMetadataFlags::Singleton))
	{
		return EClassMetadataFlags::Singleton;
	}
	
	for (const std::string& baseClass : baseClasses)
	{
		EditorTypePropertiesBase* pBaseClass = static_cast<EditorTypePropertiesBase*>(Game::GetAssetManager().FindTemplateType(baseClass));
		DOMASSERT(pBaseClass);

		EClassMetadataFlags foundFlag = pBaseClass->GetChildInstancedOrSingletonFlag();
		if (foundFlag != EClassMetadataFlags::None)
		{
			return foundFlag;
		}
	}

	return EClassMetadataFlags::None;
}
