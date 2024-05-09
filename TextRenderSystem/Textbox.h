#pragma once

#include <DomUtils/Pointers.h>

#include "TextRenderSystem.h"

class RendererObject;
class VectorArt;

/**
 * This class handles a single textbox being rendered on screen (a box of quads that have the characters on)
 */
class Textbox
{
public:
	Textbox(const TextboxParams& textboxParams, CharacterVectorArtMap& characterVectorArtMap);
	~Textbox();

	std::vector<TransientPtr<RendererObject>> rendererObjects;
	
};
