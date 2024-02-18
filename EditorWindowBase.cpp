#include "pch.h"

#include "EditorWindowBase.h"

#include "ImGuiEditor.h"

void EditorWindowBase::OpenBase()
{
	Open();
}

void EditorWindowBase::CloseBase()
{
	Close();
}

void EditorWindowBase::DrawBase()
{
	ImGui::Begin(GetWindowName().c_str(), nullptr); // #JANK: There's a crash here for whatever the fuck reason. I don't know what it is.
	bIsFocused = ImGui::IsWindowFocused();
	Draw();
	ImGui::End();
}

bool EditorWindowBase::ShouldCloseOnRequest() const
{
	return bIsFocused && CanCloseOnRequest();
}

void EditorWindowBase::RequestClose()
{
	pEditor->RemoveWindow(this);
}

