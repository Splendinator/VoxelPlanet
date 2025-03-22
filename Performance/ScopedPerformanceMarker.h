#pragma once

#define SCOPED_PERFORMANCE_MARKER(Name) ScopedPerformanceMarker ScopedPerformanceMarker##Name(#Name); (void)ScopedPerformanceMarker##Name

// Place this in a scope to measure how long it takes and put results in the PerformanceMetricsManager for debugging
class ScopedPerformanceMarker
{
public:

	ScopedPerformanceMarker(std::string inName);
	~ScopedPerformanceMarker();

	std::string name;
	std::chrono::time_point<std::chrono::high_resolution_clock> start;
};
