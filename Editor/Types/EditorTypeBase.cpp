#include "pch.h"

#include "EditorTypeBase.h"

bool EditorTypeBase::HasMetadataFlag(EClassMetadataFlags flag) const
{
	return (int)metadataFlags & (int)flag;
}

void EditorTypeBase::AddMetadataFlag(EClassMetadataFlags flag)
{
	metadataFlags = (EClassMetadataFlags)((int)metadataFlags | (int)flag);
}
