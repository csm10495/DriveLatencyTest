/*
	This file is part of DriveLatencytest (DLT).
		DLT is released via an MIT License.
			Author(s): Charles Machalow

	This is the header for stat keeping things
*/

#pragma once

#include <cstdint>

//! Contains info related to a single IO
typedef struct _IO_INFO
{
	uint64_t offsetBytes;
	uint64_t numBytes;
	uint64_t elapsedMicroseconds;
}IO_INFO;

//! Contains results for a workload
typedef struct _WORKLOAD_RESULT
{
	uint64_t maxLatencyMicroseconds;
	uint64_t minLatencyMicroseconds;
	uint64_t averageLatencyMicroseconds;
	uint64_t numIosCompleted;
	uint64_t totalBytesRead;
	uint64_t iops;
	uint64_t microsecondsDoingIo;
	uint64_t expectedWorkloadMicroseconds;
	float efficiencyPercentage;

	std::string toString() const;
}WORKLOAD_RESULT;

