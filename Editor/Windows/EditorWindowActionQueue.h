#pragma once

#include "EditorWindowBase.h"

class EditorActionBase;

/** EditorWindowActionQueue
*
* This class is used for rendering a separate ImGUI window in the editor that shows the action queue.
*/
class EditorWindowActionQueue : public EditorWindowBase
{
public:

	EditorWindowActionQueue(const std::vector<std::shared_ptr<EditorActionBase>>& inActionQueue, int& inActionIndex) : actionQueue(inActionQueue), actionIndex(inActionIndex) {}

protected:
	//~ Begin EditorWindowBase Interface
	void Draw() override;
	std::string GetWindowName() const override { return "Action Queue"; }
	bool CanCloseOnRequest() const override { return false; }
	//~ End EditorWindowBase Interface

private:
	const std::vector<std::shared_ptr<EditorActionBase>>& actionQueue;
	int& actionIndex;
};
