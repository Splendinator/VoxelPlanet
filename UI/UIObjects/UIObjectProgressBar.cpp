#include "pch.h"

#include "UIObjectProgressBar.h"

#include "VectorPrimitiveRectangle.h"

void UIObjectProgressBar::Init(VectorPrimitiveLayer* pRoot)
{
	progressBarTotalWidth = pRoot->GetBoundingBox().GetWidth();

	// pRoot will contain background and foreground, we pick out the foreground that will rescale
	pProgressBar = pRoot->FindLayerByLabel("ProgressBar");
}

void UIObjectProgressBar::Uninit()
{
	pProgressBar = nullptr;
	progressBarTotalWidth = 0.0f;
}

void UIObjectProgressBar::SetProgressFrac(float progressFrac)
{
	progressFrac = std::clamp(progressFrac, 0.0f, 1.0f);

	pProgressBar->SetScaleX(progressFrac);
}
