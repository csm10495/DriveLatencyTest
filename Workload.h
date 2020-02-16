/*
	This file is part of DriveLatencytest (DLT).
		DLT is released via an MIT License.
			Author(s): Charles Machalow

	This is the header for the workload
*/

#pragma once

#include <random>

#include "Config.h"
#include "Stats.h"

//! Workload class. Does the workload/test.
class Workload
{
public:

	/*! Constructor that will take a config
	*/
	Workload(const CONFIG& config);

	WORKLOAD_RESULT runWorkload();

private:
	// Gets the offset for the next IO
	uint64_t getNextByteOffset();

	// Gets the size for the next IO
	inline uint64_t getNextIoSize()
	{
		// will allow us to change this dynamically one day
		return nextIoSize;
	}

	//! Private copy of the config
	CONFIG config;

	//! Things to maintain a current state
	uint64_t nextByteOffset;
	uint64_t nextIoSize;

	//! Random number generation in case we need it
	std::mt19937_64 randomNumberGenerator;
	std::uniform_int_distribution<uint64_t> randomNumberDistribution;

	// disable some things
	Workload& operator=(const Workload&) = delete;
	Workload(const Workload&) = delete;
	Workload() = default;
};