/*
	This file is part of DriveLatencytest (DLT).
		DLT is released via an MIT License.
			Author(s): Charles Machalow

	This is the implementation for stat keeping things
*/

#include <algorithm>
#include <string>

#include "Stats.h"

_WORKLOAD_RESULT::_WORKLOAD_RESULT(const std::list<_WORKLOAD_RESULT>& workloadResults) : _WORKLOAD_RESULT()
{
	for (auto& result : workloadResults)
	{
		// min/max
		maxLatencyMicroseconds = std::max(result.maxLatencyMicroseconds, maxLatencyMicroseconds);
		minLatencyMicroseconds = std::min(result.minLatencyMicroseconds, minLatencyMicroseconds);

		// things to divide later
		averageLatencyMicroseconds += result.averageLatencyMicroseconds;
		microsecondsDoingIo += result.microsecondsDoingIo;
		expectedWorkloadMicroseconds += result.expectedWorkloadMicroseconds;
		efficiencyPercentage += result.efficiencyPercentage;

		//things just to add
		numIosCompleted += result.numIosCompleted;
		iops += result.iops;
		totalBytesRead += result.totalBytesRead;
	}

	// divide to get average
	averageLatencyMicroseconds /= workloadResults.size();
	microsecondsDoingIo /= workloadResults.size();
	expectedWorkloadMicroseconds /= workloadResults.size();
	efficiencyPercentage /= workloadResults.size();
}

_WORKLOAD_RESULT::_WORKLOAD_RESULT()
{
	memset(this, 0, sizeof(_WORKLOAD_RESULT));
	this->minLatencyMicroseconds = -1; // preset high
}

std::string _WORKLOAD_RESULT::toString() const
{
	std::string retStr = "";	
	
	retStr += "  Max Latency (microseconds)         : " + std::to_string(maxLatencyMicroseconds) + "\n";
	retStr += "  Min Latency (microseconds)         : " + std::to_string(minLatencyMicroseconds) + "\n";
	retStr += "  Avg Latency (microseconds)         : " + std::to_string(averageLatencyMicroseconds) + "\n";

	retStr += "  Number of Completed IOs            : " + std::to_string(numIosCompleted) + "\n";
	retStr += "  Total Bytes Read                   : " + std::to_string(totalBytesRead) + "\n";
	retStr += "  IO Per Second (IOPS)               : " + std::to_string(iops) + "\n";
	retStr += "  Microseconds Doing IO              : " + std::to_string(microsecondsDoingIo) + "\n";
	retStr += "  Microseconds Expected For Workload : " + std::to_string(expectedWorkloadMicroseconds) + "\n";
	retStr += "  Workload Efficiency                : " + std::to_string(efficiencyPercentage * 100) + "%\n";

	return retStr;
}