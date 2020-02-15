/*
	This file is part of DriveLatencytest (DLT).
		DLT is released via an MIT License.
			Author(s): Charles Machalow

	This is the implementation for the workload
*/

#include "FDFile.h"
#include "Workload.h"

#include <chrono>

Workload::Workload(const CONFIG& config)
{
	this->config = config;
}

WORKLOAD_RESULT Workload::runWorkload()
{
	WORKLOAD_RESULT workloadResult = { 0 };
	workloadResult.expectedWorkloadMicroseconds = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::seconds(config.Seconds)).count();
	workloadResult.minLatencyMicroseconds = -1; // preset high

	nextByteOffset = config.StartingOffsetInBytes;
	nextIoSize = config.IOSizeInBytes;

	FDFile drive(config.Path);

	auto end = std::chrono::high_resolution_clock::now() + std::chrono::seconds(config.Seconds);

	while (std::chrono::high_resolution_clock::now() < end)
	{
		// loop-ty loop
		ASSERT(drive.read(getNextByteOffset(), getNextIoSize()), "Failed to read");

		auto& lastIoInfo = drive.getLastIoInfo();

		workloadResult.maxLatencyMicroseconds = std::max(lastIoInfo.elapsedMicroseconds, workloadResult.maxLatencyMicroseconds);
		workloadResult.minLatencyMicroseconds = std::min(lastIoInfo.elapsedMicroseconds, workloadResult.minLatencyMicroseconds);
		workloadResult.microsecondsDoingIo += lastIoInfo.elapsedMicroseconds;
		workloadResult.numIosCompleted += 1;
		workloadResult.totalBytesRead += drive.getLastIoInfo().numBytes;
	}

	workloadResult.efficiencyPercentage = static_cast<float>(workloadResult.microsecondsDoingIo) / workloadResult.expectedWorkloadMicroseconds;
	workloadResult.iops = static_cast<uint64_t>(workloadResult.numIosCompleted / config.Seconds);
	workloadResult.averageLatencyMicroseconds = static_cast<uint64_t>(static_cast<double>(workloadResult.microsecondsDoingIo) / workloadResult.numIosCompleted);

	return workloadResult;
}