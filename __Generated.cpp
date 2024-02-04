#include "pch.h"
#include "__Generated.h"
#include "EditorAssetClass.h"
#include "EditorTypePropertyClass.h"
#include "EditorTypePropertyFloat.h"
#include "..\Roguelike\Game.h"

void* SingleFloat::InitFromAsset(EditorAssetBase* pAsset)
{
SingleFloat* pReturn = new SingleFloat;
EditorAssetClass* pEditorAssetClass = static_cast<EditorAssetClass*>(pAsset);
const std::vector<EditorTypePropertyBase*>& properties = pEditorAssetClass->GetProperties();
pReturn->value = static_cast<EditorTypePropertyFloat*>(properties[0])->GetValue();
pReturn->pNext = static_cast<SingleFloat*>(static_cast<EditorTypePropertyClass*>(properties[1])->GetValue());
return pReturn;
}

namespace __Generated
{
	std::unordered_map<std::string, void* (*)(EditorAssetBase*)> stringToCreateClassFunction
	{
		{"SingleFloat", &SingleFloat::InitFromAsset},
	};
}
