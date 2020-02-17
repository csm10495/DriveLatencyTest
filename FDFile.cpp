/*
	This file is part of DriveLatencytest (DLT).
		DLT is released via an MIT License.
			Author(s): Charles Machalow

	This is the implementation for the FDFile class
*/

#include <chrono>
#include <limits>
#include <malloc.h>
#include <stdlib.h>
#include <string.h>

#include "Assert.h"
#include "FDFile.h"

#ifdef DLT_OS_WINDOWS
// Windows
#include <io.h>
#define lseek64 _lseeki64
#define O_DIRECT 0
#define O_SYNC 0
typedef unsigned IO_SIZE_TYPE; // funny enough on Windows the numBytes for IO is an unsigned int. On POSIX it's a size_t.

// Aligned allocations
void* aligned_alloc(size_t alignment, size_t size)
{
	return _aligned_malloc(size, alignment);
}
#define aligned_free _aligned_free
#else
// Not Windows
#include <unistd.h>
#define O_BINARY 0
typedef size_t IO_SIZE_TYPE;

// Aligned allocations
#define aligned_free free
#endif

#include <fcntl.h>

/*! Funny enough the offset is a signed int since you may want to seek backward
     (in a file... not really in our case).
*/
typedef int64_t IO_OFFSET_TYPE;
const uint64_t MAX_IO_OFFSET = static_cast<uint64_t>(std::numeric_limits<IO_OFFSET_TYPE>::max());

#define MAX_IO_SIZE ((IO_SIZE_TYPE)-1)

FDFile::FDFile(const CONFIG& config)
{
	lastIoInfo = { 0 };

	handle = open(config.Path.c_str(), O_BINARY | O_RDWR | O_DIRECT | O_SYNC);
	ASSERT(handle != -1, config.Path + " did not open!... errno was: " + std::to_string(errno) + ": " + strerror(errno));

	// POSIX based systems tend to require aligned buffers for O_DIRECT.
	buffer = aligned_alloc(config.IOSizeInBytes, config.IOSizeInBytes);
	ASSERT(buffer != NULL, "Could not allocate " + std::to_string(config.IOSizeInBytes) + " bytes");
}

FDFile::~FDFile()
{
	close(handle);
	handle = -1;

	aligned_free(buffer);
	buffer = NULL;
}

bool FDFile::read(uint64_t offsetBytes, uint64_t numBytes)
{
	ASSERT((uint64_t)MAX_IO_SIZE > numBytes, "IO was requested to be larger than max allowed. Too large: " + std::to_string(numBytes));
	ASSERT(offsetBytes < MAX_IO_OFFSET, "The given byte offset was larger than the platform natively supports: " + std::to_string(offsetBytes));
	ASSERT(lseek64(handle, offsetBytes, SEEK_SET) == offsetBytes, "Could not seek to offset: " + std::to_string(offsetBytes));

	IO_SIZE_TYPE numBytesRead;

	auto start = std::chrono::high_resolution_clock::now();

	// START FAST PATH
	numBytesRead = static_cast<IO_SIZE_TYPE>(::read(handle, buffer, static_cast<IO_SIZE_TYPE>(numBytes)));
	// END FAST PATH

	auto end = std::chrono::high_resolution_clock::now();

	// Save last IO info
	lastIoInfo.numBytes = numBytes;
	lastIoInfo.offsetBytes = offsetBytes;
	lastIoInfo.elapsedMicroseconds = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

	return numBytesRead == numBytes;
}

