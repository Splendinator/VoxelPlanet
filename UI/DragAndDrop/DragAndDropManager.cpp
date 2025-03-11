#include "pch.h"

#include "DragAndDropManager.h"

#include "DirectoryData.h"
#include "DomWindow/DomWindow.h"
#include "IDragAndDropArea.h"
#include "IDragAndDroppable.h"
#include "UI/UICanvas.h"
#include "UI/UIObjects/UIObjectIconLoader.h"

void DragAndDropManager::Init()
{
	if (pDirectoryData)
	{
		UICanvasInitParams initParams;
		initParams.sizeX = floatingIconSize;
		initParams.sizeY = floatingIconSize;
		initParams.filePath = DirectoryData::ConcatenateSVGFilePathChecked(pDirectoryData->sharedUI, floatingIconFileName);
		initParams.renderPriority = RenderPriority::dragAndDropIcon;
		initParams.type = dmgf::ERenderObjectType::UI;
		initParams.bStartVisible = false;

		floatingIconCanvas = std::make_unique<UICanvas>(initParams);
		
		floatingIconLoader = floatingIconCanvas->AddExistingUIObject<UIObjectIconLoader>(iconLoaderLayerName);
		
	}
}

void DragAndDropManager::Tick(float deltaTime)
{
	if (!floatingIconCanvas || !pDirectoryData || !floatingIconLoader)
	{
		return;
	}
	
	// Handle drag
	if (pCurrentlyHeldDragAndDroppable == nullptr && dmwi::isPressed(dmwi::Button::LMB))
	{
		if (IDragAndDropArea* pArea = GetAreaUnderMouse())
		{
			IDragAndDroppable* pPossibleDroppable = nullptr;
			pArea->RecievedDragEvent(pPossibleDroppable);
			if (pPossibleDroppable)
			{
				floatingIconCanvas->SetVisible(true);
				floatingIconLoader->LoadUIIcon(pPossibleDroppable->GetDragAndDropIconFile(*pDirectoryData), pPossibleDroppable->GetOptionalDragAndDropIconLayer());
				pCurrentlyHeldDragAndDroppable = pPossibleDroppable;
			}
		}
	}

	// Handle drop
	if (pCurrentlyHeldDragAndDroppable != nullptr && dmwi::isReleased(dmwi::Button::LMB))
	{
		if (IDragAndDropArea* pArea = GetAreaUnderMouse())
		{
			pArea->RecievedDropEvent(*pCurrentlyHeldDragAndDroppable);
		}

		pCurrentlyHeldDragAndDroppable.Clear();
		floatingIconCanvas->SetVisible(false);
	}
	
	// Make floating icon follow mouse
	if (pCurrentlyHeldDragAndDroppable && floatingIconCanvas)
	{
		floatingIconCanvas->SetPosition(dmwi::GetMouseX() + floatingIconXOffset, dmwi::GetMouseY() + floatingIconYOffset);
	}
}

void DragAndDropManager::UnInit()
{
	
}

void DragAndDropManager::RegisterArea(IDragAndDropArea& area, DragAndDropAreaRegisterParams& params)
{
	DOMASSERT(params.priority != EDragAndDropAreaPriority::COUNT)

	registeredAreas[(int)params.priority].push_back(&area);
}

void DragAndDropManager::UnregisterArea(IDragAndDropArea& area)
{
	for (std::vector<IDragAndDropArea*>& areas : registeredAreas)
	{
		for (auto it = areas.begin(); it != areas.end(); ++it)
		{
			if (*it == &area)
			{
				areas.erase(it);
				return;
			}
		}
	}

	DOMLOG_WARN("Unregistering area that was never registered")
}

IDragAndDropArea* DragAndDropManager::GetAreaUnderMouse() const
{
	for (int priority = (int)EDragAndDropAreaPriority::COUNT - 1; priority >= 0; --priority)
	{
		for (IDragAndDropArea* pArea : registeredAreas[priority])
		{
			if (pArea->dragAndDropAreaBoundingBox.ContainedWithin({(float)dmwi::GetMouseX(), (float)dmwi::GetMouseY()}))
			{
				return pArea;
				
			}
		}
	}
	return nullptr;
}

#if DOMIMGUI
void DragAndDropManager::DrawImGui(float deltaTime)
{
	ImGui::Text("Mouse Pos (%d, %d)", dmwi::GetMouseX(), dmwi::GetMouseY());
	ImGui::Separator();
	
	for (int priority = (int)EDragAndDropAreaPriority::COUNT - 1; priority >= 0; --priority)
	{
		for (IDragAndDropArea* pArea : registeredAreas[priority])
		{
			ImGui::PushID(pArea);

			if (pArea->dragAndDropAreaBoundingBox.ContainedWithin({(float)dmwi::GetMouseX(), (float)dmwi::GetMouseY()}))
			{
				ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.0f, 0.6f, 0.0f, 1.0f));
			}
			else
			{
				ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.6f, 0.0f, 0.0f, 1.0f));
			}
			
			ImGui::Text("X: %f", pArea->dragAndDropAreaBoundingBox.left);
			ImGui::Text("Y: %f", pArea->dragAndDropAreaBoundingBox.top);
			ImGui::Text("Width: %f", pArea->dragAndDropAreaBoundingBox.width);
			ImGui::Text("Height: %f", pArea->dragAndDropAreaBoundingBox.height);

			ImGui::PopStyleColor();
			
			ImGui::Separator();
			ImGui::PopID();
		}
	}
}
#endif
