#pragma once

// Search bar in ImGui. Allows you to search for and select a specific string in a list of strings.
// Usage: Set the items using SetItems(), then call Draw() to render the searchable item list, then grab the selected item using GetSelectedItem().
class ImGuiItemListWithSearchBar
{
public:
	
	void SetItems(std::vector<std::string>&& items) { allItems = std::move(items); }
	void SetItems(const std::vector<std::string>& items) { allItems = items; }
	const std::string& GetSelectedItem() const { return selectedItem; }
	
	void Draw();
	
private:

	std::string selectedItem;
	
	char searchBuffer[128] = {};

	std::vector<std::string> allItems;
};
