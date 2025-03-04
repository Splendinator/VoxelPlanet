#include "pch.h"

#include "RPGSkillSystem.h"

#include "DirectoryData.h"
#include "Renderer.h"
#include "RendererObject.h"

void RPGSkillSystem::Init()
{
	if (pDirectoryData)
	{
		TransientPtr<RendererObject> transparentRenderObject = dmgf::AddObjectFromSVG(DirectoryData::ConcatenateSVGFilePathChecked(pDirectoryData->rpgSkillVisuals, greenTransparentFileName).c_str(), dmgf::ERenderObjectType::UI);
		transparentRenderObject->SetRenderPriority(1.0f);
		transparentRenderObject->SetSize(3000.f, 3000.f);
	}
}

void RPGSkillSystem::Tick(float deltaTime)
{
	
}

void RPGSkillSystem::UnInit()
{
	
}
