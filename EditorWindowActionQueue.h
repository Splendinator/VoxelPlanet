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

	//~ Begin EditorWindowBase Interface
	void Draw() override;
	//~ End EditorWindowBase Interface

private:
	const std::vector<std::shared_ptr<EditorActionBase>>& actionQueue;
	int& actionIndex;
};
