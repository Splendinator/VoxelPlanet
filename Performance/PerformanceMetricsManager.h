#pragma once

#include "Core/GameSystem.h"

struct PerformanceMetric
{
	std::string displayName;
	float timeMs;
};

EDITORCLASS()
class PerformanceMetricsManager : public GameSystem
{
	EDITORBODY()
public:
	
#ifdef DOMIMGUI
	void AddMetric(const PerformanceMetric& metric);
	
	void DrawImGui(float deltaTime) override;

	void StartRecordingMetrics();
	void StopRecordingMetrics();
	
	static constexpr int MAX_FRAMES = 4096;
	
	std::vector<PerformanceMetric> performanceMetrics[MAX_FRAMES];

	int currentFrame = 0;
	bool bRecordingPerformaceMetric = false;
#endif
};
