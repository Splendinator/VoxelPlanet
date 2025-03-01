#include "pch.h"

#include "EditorAssetBase.h"

void EditorAssetBase::OnPropertyChanged(const OnPropertyChangedData& params)
{
	onPropertyChanged.Invoke(params);
}
