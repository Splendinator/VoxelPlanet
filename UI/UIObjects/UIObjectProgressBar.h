#pragma once

#include "UIObjectBase.h"
#include "DomUtils/Pointers.h"

class VectorPrimitiveRectangle;

/** UIObjectProgressBar
*
* UI Object that scales a layer along the X direction depending on progress passed in
*/
class UIObjectProgressBar : public UIObjectBase
{
public:

	// 0.0 - 1.0
	void SetProgressFrac(float progressFrac);

protected:
	//~ Begin UIObjectBase Interface
	void Init(VectorPrimitiveLayer* pRoot) override;
	void Uninit() override;
	//~ End UIObjectBase Interface

	TransientPtr<VectorPrimitiveLayer> pProgressBar = nullptr;
	float progressBarTotalWidth = 0.0f;
};