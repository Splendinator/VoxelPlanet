#include "pch.h"

#include "PerformanceMetricsManager.h"

#ifdef DOMIMGUI
void PerformanceMetricsManager::AddMetric(const PerformanceMetric& metric)
{
	if (bRecordingPerformaceMetric)
	{
		performanceMetrics[currentFrame].insert(performanceMetrics[currentFrame].begin(), metric);
	}
}

void PerformanceMetricsManager::StartRecordingMetrics()
{
	currentFrame = 0;
	bRecordingPerformaceMetric = true;
}

void PerformanceMetricsManager::StopRecordingMetrics()
{
	bRecordingPerformaceMetric = false;
	for (std::vector<PerformanceMetric>& metric : performanceMetrics)
	{
		metric.clear();
	}
}

void PerformanceMetricsManager::DrawImGui(float deltaTime)
{
	if (bRecordingPerformaceMetric)
	{
		++currentFrame;
		
		if (currentFrame >= MAX_FRAMES)
		{
			DOMLOG_WARN("Out of frames")
			bRecordingPerformaceMetric = false;
		}
	}

	if (!bRecordingPerformaceMetric)
	{
		if (ImGui::Button("Start"))
		{
			StartRecordingMetrics();
		}
	}
	else
	{
		if (ImGui::Button("Stop"))
		{
			StopRecordingMetrics();
		}
	}
	
	std::string currentFrameHeader;
	for (int frame = 0; frame < MAX_FRAMES; ++frame)
	{
		currentFrameHeader = "Frame " + std::to_string(frame);
		if (ImGui::CollapsingHeader(currentFrameHeader.c_str(), ImGuiTreeNodeFlags_DefaultOpen))
		{
			for (PerformanceMetric& metric : performanceMetrics[frame])
			{
				ImGui::Text("%s - %fms", metric.displayName.c_str(), metric.timeMs);
			}
		}
	}
}
#endif
