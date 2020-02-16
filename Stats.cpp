/*
	This file is part of DriveLatencytest (DLT).
		DLT is released via an MIT License.
			Author(s): Charles Machalow

	This is the implementation for stat keeping things
*/

#include <string>

#include "Stats.h"

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