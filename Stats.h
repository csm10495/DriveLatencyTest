/*
	This file is part of DriveLatencytest (DLT).
		DLT is released via an MIT License.
			Author(s): Charles Machalow

	This is the header for stat keeping things
*/

#include <cstdint>

//! Contains info related to a single IO
typedef struct IO_INFO
{
	uint64_t offsetBytes;
	uint64_t numBytes;
	uint64_t elapsedMicroseconds;
}IO_INFO;