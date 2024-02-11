#include "pch.h"

#include "EditorTypeFactoryPropertiesBase.h"

#include "EditorTypePropertiesBase.h"
#include "EditorTypePropertyFactoryClass.h"
#include "EditorTypePropertyFactoryFloat.h"
#include "EditorTypePropertyFactoryStruct.h"
#include "ImGuiEditorGlobals.h"

void EditorTypeFactoryPropertiesBase::PopulateProperties(std::ifstream& stream, EditorTypePropertiesBase* pEditorTypeProperties)
{
	EditorTypePropertyFactoryFloat floatFactory;
	EditorTypePropertyFactoryClass classFactory;
	EditorTypePropertyFactoryStruct structFactory;

	EditorTypePropertyFactoryBase* propertyFactories[] = 
	{
		&floatFactory,
		&classFactory,
		&structFactory,
	};

	while (true)
	{
		std::string keyword;

		std::streampos preKeywordStreamPosition = stream.tellg();

		stream >> keyword;
		
		if(keyword == ImGuiEditorGlobals::delimiterString)
		{
			// End of properties
			break;
		}
		
		bool bFound = false;
		for (EditorTypePropertyFactoryBase* pFactory : propertyFactories)
		{
			if (pFactory->GetLabel() == keyword)
			{
				stream.seekg(preKeywordStreamPosition); // Get the keyword in too to unify how EditorTypePropertyBase::ReadFromFile() does things
				EditorTypePropertyBase* pProperty = pFactory->CreateType(stream);
				pEditorTypeProperties->pProperties.push_back(pProperty);
				bFound = true;
				break;
			}
		}

		if (!bFound)
		{
			DOMLOG_ERROR("Cannot find property with keyword", keyword);
		}
	}

	pEditorTypeProperties->OnPropertiesPopulated();
}
