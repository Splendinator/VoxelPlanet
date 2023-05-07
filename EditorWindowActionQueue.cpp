#include "pch.h"

#include "EditorWindowActionQueue.h"

#include "EditorActionBase.h"
#include "ImGuiEditor.h"

void EditorWindowActionQueue::Draw()
{
	ImGui::Begin("Action Queue", nullptr);

	auto DrawUndoRedoButtons = [this]()
	{
		if (ImGui::Button("Undo"))
		{
			pEditor->Undo();
		}

		ImGui::SameLine();
		if (ImGui::Button("Redo"))
		{
			pEditor->Redo();
		}
	};

	for (int i = static_cast<int>(actionQueue.size()) - 1; i >= 0; --i)
	{
		const bool bGreyOut = i > actionIndex; // Grey out the text if the action is undone but can be re-done
		if (bGreyOut)
		{
			ImVec4 greyedOutTextColor(0.5f, 0.5f, 0.5f, 1.0f);
			ImGui::PushStyleColor(ImGuiCol_Text, greyedOutTextColor);
		}

		std::string actionDescription = actionQueue[i]->GetDescription();
		ImGui::Text("%s", actionDescription.c_str());

		if (bGreyOut)
		{
			ImGui::PopStyleColor();
		}

		if (static_cast<int>(i) == actionIndex)
		{
			ImGui::SameLine();
			DrawUndoRedoButtons(); // Draw undo - redo buttons next to the current action
		}
	}

	if (actionIndex == -1)
	{
		// If no current action, just draw undo redo buttons at the bottom
		DrawUndoRedoButtons();
	}

	ImGui::End();
}