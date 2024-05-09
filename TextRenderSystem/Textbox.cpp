#include "pch.h"

#include "Textbox.h"

#include "Renderer.h"
#include "RendererObject.h"
#include "TextRenderSystem.h"

// #TODO: Replace programmer art first-pass letters with actual letters when we have better vector art primitives
// #TODO: Wrap text around a max width
// #TODO: Add per-letter specified "padding" between letters to keep spacing correct
// #TODO: Add global horizontal/vertical spacing etc.

Textbox::Textbox(const TextboxParams& textboxParams, CharacterVectorArtMap& characterVectorArtMap)
{
	rendererObjects.reserve(textboxParams.text.length());
	
	float currentXPos = textboxParams.xPos;
	float currentYPos = textboxParams.yPos;
	for (char characterToRender : textboxParams.text)
	{
		if (characterToRender == '\0')
		{
			break;
		}
		
		if (const std::shared_ptr<VectorArt>& pVectorArt = characterVectorArtMap[characterToRender])
		{
			TransientPtr<RendererObject> pRendererObject = dmgf::AddObjectFromVectorArt(pVectorArt, dmgf::ERenderObjectType::HUD);
			pRendererObject->SetPosition(currentXPos, currentYPos);
			pRendererObject->SetSize(textboxParams.fontSize,textboxParams.fontSize);
			rendererObjects.push_back(pRendererObject);
		}
		else
		{
			DOMLOG_WARN("No .svg for character", characterToRender)
		}

		currentXPos += textboxParams.fontSize;
	}
}

Textbox::~Textbox()
{
	for (RendererObject* pRendererObject : rendererObjects)
	{
		dmgf::RemoveObject(pRendererObject);
	}
}
