#include "pch.h"

#include "EditorTypePropertyFloat.h"

void EditorTypePropertyFloat::DrawImGUI()
{
	ImGui::InputFloat(name.c_str(), &value);
}

