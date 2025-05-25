#include "pch.h"

#include "ImGuiItemListWithSearchBar.h"

void ImGuiItemListWithSearchBar::Draw()
{
	ImGui::PushItemWidth(-1);
	ImGui::InputTextWithHint("Search", "Search Classes", searchBuffer, sizeof(searchBuffer));
	ImGui::PopItemWidth();
	
	char lowerCaseSearchBuffer[128] = {};
	memcpy(lowerCaseSearchBuffer, searchBuffer, sizeof(searchBuffer));
	dmut::ToLowerInline(&lowerCaseSearchBuffer[0]);
	
	ImGui::BeginChild("Classes", ImVec2(0, 200), true);
	for (const std::string& type : allItems)
	{
		if (searchBuffer[0] == '\0' || std::strstr(dmut::ToLower(type).c_str(), lowerCaseSearchBuffer) != nullptr)
		{
			bool bSelected = selectedItem == type;
			if (ImGui::Selectable(type.c_str(), bSelected))
			{
				selectedItem = type;
			}
		}
	}
	ImGui::EndChild();
}
