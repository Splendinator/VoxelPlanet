#pragma once

#include <string>

enum class EEditorActionState
{
	Done, // Action has been executed and can be undone
	NotDone, // Action hasn't been done (either because it was never executed or because it was undone) 
};


/** EditorActionBase
*
* This class is used as the base class for editor actions that can be undoable and redoable.
*/
class EditorActionBase
{
public:
	
	virtual ~EditorActionBase() {}

	void BaseUndo();
	bool BaseTryExecuteAction();
	
	virtual std::string GetDescription() const = 0;

protected:

	virtual void Undo() = 0;
	virtual bool TryExecuteAction() = 0;
	
	EEditorActionState state = EEditorActionState::NotDone;
};
