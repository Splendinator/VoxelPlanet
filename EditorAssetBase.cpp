#include "pch.h"

#include "EditorAssetBase.h"

void EditorAssetBase::OnPropertyChanged(const OnPropertyChangedParams& params)
{
	onPropertyChanged.Invoke(params);
}
