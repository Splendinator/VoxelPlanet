#pragma once

#include "UIObjectBase.h"
#include "DomUtils/Pointers.h"

class VectorPrimitiveRectangle;

/** UIObjectProgressBar
*
* UI Object that resizes a VectorPrimitiveRectangle to allow setting progress
*/
class UIObjectProgressBar : public UIObjectBase
{
public:

	// 0.0 - 1.0
	void SetProgressFrac(float progressFrac);

protected:
	//~ Begin UIObjectBase Interface
	void Init(VectorPrimitiveBase* pRoot) override;
	void Uninit() override;
	//~ End UIObjectBase Interface

	TransientPtr<VectorPrimitiveRectangle> pProgressBarPrimitive = nullptr;
	float progressBarTotalWidth = 0.0f;
};