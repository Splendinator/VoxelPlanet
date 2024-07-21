#include "pch.h"

#include "EditorTypePropertiesBase.h"

#include "EditorTypePropertyBase.h"

// #TEMP: Optimisation
#pragma optimize("", off)
EditorTypePropertiesBase::~EditorTypePropertiesBase()
{
	for (EditorTypePropertyBase* pProperty : pProperties)
	{
		delete pProperty;
	}
	pProperties.clear();
}
#pragma optimize("", on)

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

void EditorTypePropertiesBase::DeepCopyProperties(EditorTypePropertiesBase* pOther)
{
	for (EditorTypePropertyBase* pProperty : pProperties)
	{
		pOther->pProperties.push_back(pProperty->DeepCopy());
	}
	pOther->OnPropertiesPopulated();
}

void EditorTypePropertiesBase::OnPropertyChanged(const OnPropertyChangedParams& params)
{
	onPropertyChanged.Invoke(params);
}
