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
	DOMASSERT(progressFrac >= 0.0f && progressFrac <= 1.0f, "Progress frac should be between 0.0 and 1.0");

	const u32 progressBarWidth = (u32)(progressBarTotalWidth * progressFrac);

	pProgressBarPrimitive->SetWidth(progressBarWidth);
}
