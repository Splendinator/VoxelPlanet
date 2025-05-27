#include "pch.h"

#include "EditorActionBase.h"

void EditorActionBase::BaseUndo()
{
	Undo();
	state = EEditorActionState::NotDone;
}

bool EditorActionBase::BaseTryExecuteAction()
{
	const bool bSuccess = TryExecuteAction();
	state = bSuccess ? EEditorActionState::Done : EEditorActionState::NotDone;
	return bSuccess;
}
