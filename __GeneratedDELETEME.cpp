/*
#include "pch.h"

#include "__Generated.h"

#include "EditorAssetClass.h"
#include "EditorTypePropertyFloat.h"

namespace __Generated
{
	std::unordered_map<std::string, void* (*)(EditorAssetBase*)> stringToCreateClassFunction
	{
		{"TestClassTwoElectricBoogaloo", &TestClassTwoElectricBoogaloo::InitFromAsset}
	};
}

/// #TEMP: Delete stuff in here (move to __Generated.cpp?)
void* TestClassTwoElectricBoogaloo::InitFromAsset(EditorAssetBase* pAsset)
{
	TestClassTwoElectricBoogaloo* pReturn = new TestClassTwoElectricBoogaloo; 
	
	EditorAssetClass* pEditorAssetClass = static_cast<EditorAssetClass*>(pAsset);
	const std::vector<EditorTypePropertyBase*>& properties = pEditorAssetClass->GetProperties();

	pReturn->damage = static_cast<EditorTypePropertyFloat*>(properties[0])->GetValue();
	pReturn->range = static_cast<EditorTypePropertyFloat*>(properties[1])->GetValue();
	pReturn->damageType = static_cast<EditorTypePropertyFloat*>(properties[2])->GetValue();

	return pReturn;	
}
*/