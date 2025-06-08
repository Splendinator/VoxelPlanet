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
		
		PathGridIntersectionPoint intersectPoint = PathUtils::GetGridIntersectionPointOfLine({lineStartX, lineStartY}, {lineEndX, lineEndY}, {targetGridX, targetGridY});
		
		ImGui::Text("Entry Point: (%f, %f)", intersectPoint.entryPoint.x, intersectPoint.entryPoint.y);
		ImGui::Text("Exit Point: (%f, %f)", intersectPoint.exitPoint.x, intersectPoint.exitPoint.y);

		const ImVec2 drawPos = ImGui::GetCursorScreenPos();
		constexpr float DRAW_SIZE_MULT = 100.0f; 
		
		ImDrawList* drawList = ImGui::GetWindowDrawList();
		const ImVec2 targetPosTopLeft = {drawPos.x + (float)targetGridX * DRAW_SIZE_MULT, drawPos.y + (float)targetGridY * DRAW_SIZE_MULT};
		const ImVec2 targetPosBottomRight = {drawPos.x + (float)targetGridX* DRAW_SIZE_MULT + DRAW_SIZE_MULT, drawPos.y + (float)targetGridY * DRAW_SIZE_MULT + DRAW_SIZE_MULT};
		
		drawList->AddRect(targetPosTopLeft, targetPosBottomRight, IM_COL32(255, 0, 0, 255));
		if (intersectPoint.DoesEnter())
		{
			drawList->AddCircle({drawPos.x + intersectPoint.entryPoint.x * DRAW_SIZE_MULT, drawPos.y + intersectPoint.entryPoint.y * DRAW_SIZE_MULT}, 10.f, IM_COL32(0, 255, 0, 255));
		}
		if (intersectPoint.DoesExit())
		{
			drawList->AddCircle({drawPos.x + intersectPoint.exitPoint.x * DRAW_SIZE_MULT, drawPos.y + intersectPoint.exitPoint.y * DRAW_SIZE_MULT}, 10.f, IM_COL32(0, 0, 255, 255));
		}
		
		drawList->AddLine(
			{drawPos.x + (float)lineStartX * DRAW_SIZE_MULT + 0.5f * DRAW_SIZE_MULT, drawPos.y + (float)lineStartY * DRAW_SIZE_MULT + 0.5f * DRAW_SIZE_MULT},
			{drawPos.x + (float)lineEndX * DRAW_SIZE_MULT + 0.5f * DRAW_SIZE_MULT, drawPos.y + (float)lineEndY * DRAW_SIZE_MULT + 0.5f * DRAW_SIZE_MULT},
			IM_COL32(255, 255, 255, 255)
		);
			
		
		ImGui::TreePop();
	}
}
