#include "pch.h"

#include "TestSuiteGameSystem.h"

#include "AI/Paths/PathUtils.h"

void TestSuiteGameSystem::DrawImGui(float deltaTime)
{
	ImGui::Text("Just used for debug testing a bunch of bollocks, make sure to delete this before release");

	ImGui::Separator();
	
	if (ImGui::TreeNode("PathUtils::GetGridIntersectionPointOfLine()"))
	{
		static int lineStartX;
		static int lineStartY;
		static int lineEndX;
		static int lineEndY;
		static int targetGridX;
		static int targetGridY;
		
		ImGui::InputInt("Line Start X", &lineStartX);
		ImGui::InputInt("Line Start Y", &lineStartY);
		ImGui::InputInt("Line End X", &lineEndX);
		ImGui::InputInt("Line End Y", &lineEndY);
		ImGui::InputInt("Target Grid X", &targetGridX);
		ImGui::InputInt("Target Grid Y", &targetGridY);
		
		PathGridIntersectionPoints intersectPoints = PathUtils::GetGridIntersectionPointOfLine({lineStartX, lineStartY}, {lineEndX, lineEndY}, {targetGridX, targetGridY});
		
		ImGui::Text("Entry Point: (%f, %f)", intersectPoints.entryPoint.x, intersectPoints.entryPoint.y);
		ImGui::Text("Exit Point: (%f, %f)", intersectPoints.exitPoint.x, intersectPoints.exitPoint.y);

		ImGui::Text("Exit Direction: ");
		ImGui::SameLine();
		switch (intersectPoints.exitDirection)
		{
		case EIntersectionExitDirectionFlags::None:
		    ImGui::Text("None");
		    break;
		case EIntersectionExitDirectionFlags::Top:
		    ImGui::Text("Top");
		    break;
		case EIntersectionExitDirectionFlags::Bottom:
		    ImGui::Text("Bottom");
		    break;
		case EIntersectionExitDirectionFlags::Left:
		    ImGui::Text("Left");
		    break;
		case EIntersectionExitDirectionFlags::Right:
		    ImGui::Text("Right");
		    break;
		case EIntersectionExitDirectionFlags::TopLeft:
		    ImGui::Text("Top-Left");
		    break;
		case EIntersectionExitDirectionFlags::TopRight:
		    ImGui::Text("Top-Right");
		    break;
		case EIntersectionExitDirectionFlags::BottomLeft:
		    ImGui::Text("Bottom-Left");
		    break;
		case EIntersectionExitDirectionFlags::BottomRight:
		    ImGui::Text("Bottom-Right");
		    break;
		default:
		    ImGui::Text("What the fuck is this? (%d)", (int)intersectPoints.exitDirection);
		    break;
		}
		
		const ImVec2 drawPos = ImGui::GetCursorScreenPos();
		constexpr float DRAW_SIZE_MULT = 100.0f; 
		
		ImDrawList* drawList = ImGui::GetWindowDrawList();
		const ImVec2 targetPosTopLeft = {drawPos.x + (float)targetGridX * DRAW_SIZE_MULT, drawPos.y + (float)targetGridY * DRAW_SIZE_MULT};
		const ImVec2 targetPosBottomRight = {drawPos.x + (float)targetGridX* DRAW_SIZE_MULT + DRAW_SIZE_MULT, drawPos.y + (float)targetGridY * DRAW_SIZE_MULT + DRAW_SIZE_MULT};
		
		drawList->AddRect(targetPosTopLeft, targetPosBottomRight, IM_COL32(255, 0, 0, 255));
		if (intersectPoints.DoesEnter())
		{
			drawList->AddCircle({drawPos.x + intersectPoints.entryPoint.x * DRAW_SIZE_MULT, drawPos.y + intersectPoints.entryPoint.y * DRAW_SIZE_MULT}, 10.f, IM_COL32(0, 255, 0, 255));
		}
		if (intersectPoints.DoesExit())
		{
			drawList->AddCircle({drawPos.x + intersectPoints.exitPoint.x * DRAW_SIZE_MULT, drawPos.y + intersectPoints.exitPoint.y * DRAW_SIZE_MULT}, 10.f, IM_COL32(0, 0, 255, 255));
		}
		
		drawList->AddLine(
			{drawPos.x + (float)lineStartX * DRAW_SIZE_MULT + 0.5f * DRAW_SIZE_MULT, drawPos.y + (float)lineStartY * DRAW_SIZE_MULT + 0.5f * DRAW_SIZE_MULT},
			{drawPos.x + (float)lineEndX * DRAW_SIZE_MULT + 0.5f * DRAW_SIZE_MULT, drawPos.y + (float)lineEndY * DRAW_SIZE_MULT + 0.5f * DRAW_SIZE_MULT},
			IM_COL32(255, 255, 255, 255)
		);
		
		ImGui::TreePop();
	}
}
