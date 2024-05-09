#pragma once

#include <string>

/** EditorActionBase
*
* This class is used as the base class for editor actions that can be undoable and redoable.
*/
class EditorActionBase
{
public:
	
	virtual ~EditorActionBase() {}

	virtual void Undo() = 0;
	virtual bool TryExecuteAction() = 0;
	virtual std::string GetDescription() const = 0;
};
