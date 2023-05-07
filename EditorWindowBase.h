#pragma once

class ImGuiEditor;

/** EditorWindowBase
*
* This class is used for rendering a seperate ImGUI window in the editor.
* The editor maintains a list of all open windows and ticks them every frame.
*/
class EditorWindowBase
{
public:
	
	virtual ~EditorWindowBase() {}

	// Children override this to draw what you want
	virtual void Draw() = 0;

	void Init(ImGuiEditor& inEditor) { pEditor = &inEditor; }

protected:

	void Close();

	// pointer to editor, assume this is always valid
	ImGuiEditor* pEditor = nullptr;

private:

};
