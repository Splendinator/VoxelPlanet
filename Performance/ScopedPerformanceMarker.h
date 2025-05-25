#pragma once

// Warning 4456 prevents duplicate names
#define SCOPED_PERFORMANCE_MARKER(displayName) _Pragma("warning( disable : 4456 )")\
	ScopedPerformanceMarker perfMarker(displayName);\
	_Pragma("warning( default : 4456 )")
	
#define SCOPED_FUNCTION_PERFORMANCE_MARKER() SCOPED_PERFORMANCE_MARKER(__FUNCTION__)

// Place this in a scope to measure how long it takes and put results in the PerformanceMetricsManager for debugging
class ScopedPerformanceMarker
{
public:

	ScopedPerformanceMarker(std::string inName);
	~ScopedPerformanceMarker();

	std::string name;
	std::chrono::time_point<std::chrono::high_resolution_clock> start;
};
