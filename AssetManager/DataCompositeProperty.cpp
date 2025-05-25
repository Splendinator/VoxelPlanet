#include "pch.h"

#include "DataCompositeProperty.h"

#include "Editor/Assets/EditorAssetDataComposite.h"

EditorTypePropertyBase* DataCompositePropertyUtils::GetPropertyFromString(std::string value)
{
	size_t pos = value.find("::");

	DOMLOG_ERROR_IF(pos == std::string::npos, "Expected '::' in value for DataCompositeProperty", value);
	
	std::string assetName = value.substr(0, pos);
	std::string propertyName = value.substr(pos + 2); // Skip over the "::"

	if (assetName != "nullptr" && propertyName != "none")
	{
		std::weak_ptr<EditorAssetDataComposite> pFoundAsset = Game::GetAssetManager().FindAsset<EditorAssetDataComposite>(assetName);
		DOMLOG_ERROR_IF(pFoundAsset.expired(), "Can't find EditorAssetDataComposite", assetName, ". This might happen after a rename?");

		if (!pFoundAsset.expired())
		{
			EditorTypePropertyBase* pEditorTypeProperty = pFoundAsset.lock()->FindProperty(propertyName);
			DOMLOG_ERROR_IF(pEditorTypeProperty == nullptr, "Cannot find property", propertyName, "in asset", assetName, ". This shouldn't happen");
			return pEditorTypeProperty;
		}
	}

	return nullptr;
}

