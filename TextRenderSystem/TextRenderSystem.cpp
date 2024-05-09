#include "pch.h"

#include "TextRenderSystem.h"

#include "DirectoryData.h"
#include "Game.h"
#include "Textbox.h"
#include "VectorArt.h"

void TextRenderSystem::Init()
{
	std::string baseDirectory = "./";
	if (Game::GetGameAssets().pDirectoryData)
	{
		baseDirectory = Game::GetGameAssets().pDirectoryData->fonts;
	}
	
	for (const TextRenderCharacterData& characterData : characterDatas)
	{
		DOMLOG_ERROR_IF(characterData.character > 128 || characterData.character < 0, characterData.character, "is out of range for ascii character", characterData.fileName);

		characterVectorArtMap[characterData.character] = std::make_shared<VectorArt>((baseDirectory + characterData.fileName + ".svg").c_str());
	}
	characterDatas.clear(); // Don't need this once loaded into our hashmap
	
}

void TextRenderSystem::Uninit()
{
	for (Textbox*& pTextbox : textboxes)
	{
		delete pTextbox;
		pTextbox = nullptr;
	}
}

TextboxHandle TextRenderSystem::AddTextbox(const TextboxParams& textboxParams)
{
	for (int index = 0; index < textboxes.size(); ++index)
	{
		Textbox*& pTextbox = textboxes[index];

		if (pTextbox == nullptr)
		{
			pTextbox = new Textbox(textboxParams, characterVectorArtMap);
			return index;
		}
	}

	textboxes.push_back(new Textbox(textboxParams, characterVectorArtMap));
	return (int)textboxes.size() - 1;
}

void TextRenderSystem::RemoveTextbox(TextboxHandle handle)
{
	DOMLOG_ERROR_IF((textboxes.size() <= handle) || handle < 0, "handle out of range", handle, textboxes.size())

	Textbox*& pTextbox = textboxes[handle];
	delete pTextbox;
	pTextbox = nullptr;
}

