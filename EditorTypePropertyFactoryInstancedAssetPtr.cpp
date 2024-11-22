#include "pch.h"

#include "EditorTypePropertyFactoryInstancedAssetPtr.h"

#include "EditorTypePropertyInstancedAssetPtr.h"

EditorTypePropertyBase* EditorTypePropertyFactoryInstancedAssetPtr::CreateType(std::ifstream& stream)
{
	EditorTypePropertyInstancedAssetPtr* pProperty = new EditorTypePropertyInstancedAssetPtr();
	pProperty->ReadFromFile(stream);

	return pProperty;
}

std::string EditorTypePropertyFactoryInstancedAssetPtr::GetLabel()
{
	return "instancedassetptr";	
}
