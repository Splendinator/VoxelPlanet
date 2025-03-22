#include "pch.h"

#include "ScopedPerformanceMarker.h"

#include "PerformanceMetricsManager.h"
#include "Core/GameInstance.h"

ScopedPerformanceMarker::ScopedPerformanceMarker(std::string inName): name(inName)
{
	start = std::chrono::high_resolution_clock::now();
}

ScopedPerformanceMarker::~ScopedPerformanceMarker()
{
	auto end = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
	if (PerformanceMetricsManager* pPerformanceManager = Game::GetGameInstance().FindGameSystemSlow<PerformanceMetricsManager>())
	{
		PerformanceMetric metric = {};
		metric.displayName = name;
		metric.timeMs = (float)duration.count() / 1000000.f;
		pPerformanceManager->AddMetric(metric);
	}
}