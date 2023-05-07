#include "pch.h"

#include "EditorWindowBase.h"

#include "ImGuiEditor.h"

void EditorWindowBase::Close()
{
	pEditor->RemoveWindow(this);
}

