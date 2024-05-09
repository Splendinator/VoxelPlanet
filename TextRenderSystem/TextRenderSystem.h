#pragma once

#include <array>

class Textbox;
class VectorArt;

typedef int TextboxHandle;

using CharacterVectorArtMap = std::array<std::shared_ptr<VectorArt>, 128>;

EDITORSTRUCT()
struct TextboxParams
{
	EDITORBODY()

	EDITORPROPERTY()
	std::string text;

	EDITORPROPERTY()
	float xPos;
	
	EDITORPROPERTY()
	float yPos;

	EDITORPROPERTY()
	float fontSize;

	// #TODO: Width etc.
	// #TODO: Editor expose this likely.
};

EDITORSTRUCT()
struct TextRenderCharacterData
{
	EDITORBODY()

	// Name of the .svg file of this character. Don't include ".svg"
	EDITORPROPERTY()
	std::string fileName;

	// ASCII code of this character
	EDITORPROPERTY()
	int character;
};

/** TextRenderSystem
 *
 * System used to render text on-screen in the game
 */
EDITORCLASS()
class TextRenderSystem
{
	EDITORBODY()
	
	// data of each character
	EDITORPROPERTY()
	std::vector<TextRenderCharacterData> characterDatas;
	
	void Init();
	void Uninit();

	TextboxHandle AddTextbox(const TextboxParams& textboxParams);
	void RemoveTextbox(TextboxHandle handle);

	
private:
	// Map of characters (ascii codes) to their vector arts 
	CharacterVectorArtMap characterVectorArtMap;

	// List of textboxes. When textboxes are removed they are simply nulled out, *not* actually removed.
	// (removing properly would mess up textbox handles i.e indexes)
	std::vector<Textbox*> textboxes;
};
