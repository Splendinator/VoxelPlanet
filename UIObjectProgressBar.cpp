#include "pch.h"

#include "UIObjectProgressBar.h"

#include "VectorPrimitiveRectangle.h"

void UIObjectProgressBar::Init(VectorPrimitiveBase* pRoot)
{
	pProgressBarPrimitive = pRoot->FindPrimitiveByLabel<VectorPrimitiveRectangle>("ProgressBar");
	progressBarTotalWidth = (float)pProgressBarPrimitive->GetWidth();
}

void UIObjectProgressBar::Uninit()
{
	pProgressBarPrimitive = nullptr;
	progressBarTotalWidth = 0.0f;
}

void UIObjectProgressBar::SetProgressFrac(float progressFrac)
{
	progressFrac = std::clamp(progressFrac, 0.0f, 1.0f);

	const u32 progressBarWidth = (u32)(progressBarTotalWidth * progressFrac);

	pProgressBarPrimitive->SetWidth(progressBarWidth);
}
