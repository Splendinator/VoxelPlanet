#pragma once

class DirectoryData;

// Interface classes can use to work with the drag and drop system.
// This should be used by the thing actually being dragged and dropped (like spells or equipment etc.)
EDITORCLASS(Abstract)
class IDragAndDroppable
{
	EDITORBODY()
public:

	// Override these to get the file + icon layer combo of the icon we want to show hovering under the mouse while this is being dragged around.
	virtual std::string GetDragAndDropIconFile(DirectoryData& directoryData) const { PUREVIRTUAL() return ""; }
	virtual std::string GetOptionalDragAndDropIconLayer() const { return ""; }
	
};
