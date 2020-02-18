/*
	This file is part of DriveLatencytest (DLT).
		DLT is released via an MIT License.
			Author(s): Charles Machalow

	This is the implementation for config keeping things
*/

#include "Config.h"

std::string _CONFIG::toString() const
{
	std::string retStr = "";

	retStr += "  Duration (Seconds)                 : " + std::to_string(Seconds) + "\n";
	retStr += "  Number Of Threads                  : " + std::to_string(ThreadCount) + "\n";
	retStr += "  IO Size (Bytes)                    : " + std::to_string(IOSizeInBytes) + "\n";
	retStr += "  Starting Offset (Bytes)            : " + std::to_string(StartingOffsetInBytes) + "\n";
	retStr += "  Ending Offset (Bytes)              : " + std::to_string(EndingOffsetInBytes) + "\n";
	retStr += "  Max IOs Per Second                 : " + std::to_string(MaxIOsPerSecond) + "\n";
	retStr += "  Workload Type                      : " + ::toString(WorkloadType) + "\n";
	retStr += "  Path                               : " + Path + "\n";

	return retStr;
}

std::vector<CONFIG> _CONFIG::splitForThreads(const uint32_t numThreads) const
{
	std::vector<CONFIG> configs;

	CONFIG config = *this;

	auto remaining = config.MaxIOsPerSecond % numThreads;
	config.MaxIOsPerSecond = static_cast<uint64_t>(static_cast<double>(config.MaxIOsPerSecond) / numThreads);

	for (uint32_t i = 0; i < numThreads; i++)
	{
		configs.push_back(config);
	}

	// first one will handle the leftover iops
	configs[0].MaxIOsPerSecond += remaining;

	return configs;
}
