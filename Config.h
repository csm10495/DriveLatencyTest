/*
	This file is part of DriveLatencytest (DLT).
		DLT is released via an MIT License.
			Author(s): Charles Machalow

	This is the header for config keeping things
*/

#pragma once

#include "Assert.h"

#include <algorithm>
#include <cstdint>
#include <string>

enum class WorkloadTypeEnum
{
	Unknown,
	Sequential,
	Random
};

inline WorkloadTypeEnum toWorkloadTypeEnum(std::string s)
{
	// convert string to upper case
	std::for_each(s.begin(), s.end(), [](char& c) {
		c = ::toupper(c);
		});

	if (s == "SEQUENTIAL")
	{
		return WorkloadTypeEnum::Sequential;
	}
	else if (s == "RANDOM")
	{
		return WorkloadTypeEnum::Random;
	}

	ASSERT(0, s + " is not a valid WorkloadTypeEnum");

	return WorkloadTypeEnum::Unknown;
}

inline std::string toString(WorkloadTypeEnum w)
{
	if (w == WorkloadTypeEnum::Unknown)
	{
		return "Unknown";
	}
	else if (w == WorkloadTypeEnum::Sequential)
	{
		return "Sequential";
	}
	else if (w == WorkloadTypeEnum::Random)
	{
		return "Random";
	}

	ASSERT(0, std::to_string((int)w) + " is not a valid workload type");
	return "Unknown";
}

//! Contains info related to the workload configuration
typedef struct _CONFIG
{
	uint32_t Seconds;
	uint32_t ThreadCount;
	uint64_t IOSizeInBytes;
	uint64_t StartingOffsetInBytes;
	uint64_t EndingOffsetInBytes;
	WorkloadTypeEnum WorkloadType;
	std::string Path;

	std::string toString() const;
}CONFIG;