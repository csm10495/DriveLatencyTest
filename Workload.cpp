/*
	This file is part of DriveLatencytest (DLT).
		DLT is released via an MIT License.
			Author(s): Charles Machalow

	This is the implementation for the workload
*/

#include "FDFile.h"
#include "Workload.h"

#include <algorithm>
#include <chrono>
#include <thread>

#define NOW std::chrono::high_resolution_clock::now()

Workload::Workload(const CONFIG& config)
{
	this->config = config;
	
	// setup random number generation
	this->randomNumberGenerator = std::mt19937_64(std::random_device()());
	this->randomNumberDistribution = std::uniform_int_distribution<uint64_t>(this->config.StartingOffsetInBytes, this->config.EndingOffsetInBytes - this->config.IOSizeInBytes);
}

WORKLOAD_RESULT Workload::runWorkload()
{
	WORKLOAD_RESULT workloadResult;
	workloadResult.expectedWorkloadMicroseconds = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::seconds(config.Seconds)).count();
	workloadResult.minLatencyMicroseconds = -1; // preset high

	nextByteOffset = config.StartingOffsetInBytes;
	nextIoSize = config.IOSizeInBytes;

	FDFile drive(config);

	auto end = NOW + std::chrono::seconds(config.Seconds);

	uint64_t iosThisSecond = 0;
	auto thisSecondStart = NOW;
	auto thisSecondEnd = NOW + std::chrono::seconds(1);

	while (NOW < end)
	{
		// loop-ty loop
		ASSERT(drive.read(getNextByteOffset(), getNextIoSize()), "Failed to read");

		auto& lastIoInfo = drive.getLastIoInfo();

		workloadResult.maxLatencyMicroseconds = std::max(lastIoInfo.elapsedMicroseconds, workloadResult.maxLatencyMicroseconds);
		workloadResult.minLatencyMicroseconds = std::min(lastIoInfo.elapsedMicroseconds, workloadResult.minLatencyMicroseconds);
		workloadResult.microsecondsDoingIo += lastIoInfo.elapsedMicroseconds;
		workloadResult.numIosCompleted += 1;
		workloadResult.totalBytesRead += drive.getLastIoInfo().numBytes;

		// start io per second handling ////
		iosThisSecond += 1;
		if (iosThisSecond >= config.MaxIOsPerSecond)
		{
			// will exit this loop when thisSecond is over
			while (NOW < thisSecondEnd)
			{
				std::this_thread::sleep_until(thisSecondEnd);
			}
		}

		// if we're onto the next second reset our markers
		if (NOW > thisSecondEnd)
		{
			iosThisSecond = 0;
			thisSecondStart = NOW;
			thisSecondEnd = NOW + std::chrono::seconds(1);
		}
		// end io per second handling    ////
	}

	workloadResult.efficiencyPercentage = std::min(static_cast<double>(100), static_cast<double>(workloadResult.microsecondsDoingIo) / workloadResult.expectedWorkloadMicroseconds);
	workloadResult.iops = static_cast<uint64_t>(workloadResult.numIosCompleted / config.Seconds);
	workloadResult.averageLatencyMicroseconds = static_cast<uint64_t>(static_cast<double>(workloadResult.microsecondsDoingIo) / workloadResult.numIosCompleted);

	return workloadResult;
}

uint64_t Workload::getNextByteOffset()
{
	auto ret = nextByteOffset;

	if (config.WorkloadType == WorkloadTypeEnum::Sequential)
	{
		// handle if we wrap around
		nextByteOffset += getNextIoSize();
		if (nextByteOffset > config.EndingOffsetInBytes)
			[[unlikely]]
		{
			nextByteOffset = config.StartingOffsetInBytes;
		}

		return ret;
	}
	else if (config.WorkloadType == WorkloadTypeEnum::Random)
	{
		// the number must be divisible by the io size. (technically it should be by the sector size but we don't know that).
		return (randomNumberDistribution(randomNumberGenerator) / config.IOSizeInBytes) * config.IOSizeInBytes;
	}

	ASSERT(0, std::to_string((int)config.WorkloadType) + " is not a valid workload type");
	return -1;
}
