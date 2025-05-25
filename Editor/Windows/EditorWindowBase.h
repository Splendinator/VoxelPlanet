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
	
	void Init(ImGuiEditor& inEditor) { pEditor = &inEditor; }
	
	void OpenBase(); // When opened
	void CloseBase(); // When closed
	void DrawBase(); // Every frame while opened

	// Whether this window should close if requested
	bool ShouldCloseOnRequest() const;

	// Text that will appear on the window, Two windows with the same name can't be open at the same time. The old one will close.
	virtual std::string GetWindowName() const = 0;
	
protected:

	// Set up anything that needs to be set up
	virtual void Open() {};
	
	// Opposite of Open()
	virtual void Close() {};

	// Children override this to draw what you want
	virtual void Draw() = 0;
	
	// Whether this window can close by pressing ESC
	virtual bool CanCloseOnRequest() const { return true; }

	// Close the screen
	void RequestClose();

	// pointer to editor, assume this is always valid
	ImGuiEditor* pEditor = nullptr;

private:

	bool bIsFocused = false;

};
