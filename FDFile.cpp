/*
	This file is part of DriveLatencytest (DLT).
		DLT is released via an MIT License.
			Author(s): Charles Machalow

	This is the implementation for the FDFile class
*/

#include <chrono>
#include <string.h>

#include "Assert.h"
#include "FDFile.h"

#ifdef DLT_OS_WINDOWS
// Windows
#include <io.h>
#define lseek64 _lseeki64
#else
// Not Windows
#include <unistd.h>
#define O_BINARY 0
#endif

#include <fcntl.h>

// Define FILE_BUFFER_SIZE to 8MB, though I guess a user can change it if they want via this macro.
#ifndef FILE_BUFFER_SIZE
#define FILE_BUFFER_SIZE (1024 * 1024 * 8)
#endif 

// funny enough on Windows the numBytes for IO is an unsigned int. On POSIX it's a size_t.
#ifdef OS_DLT_WINDOWS
// Windows
typedef unsigned IO_SIZE_TYPE;
#else
// Not Windows
typedef size_t IO_SIZE_TYPE;
#endif

#define MAX_IO_SIZE ((IO_SIZE_TYPE)-1)

FDFile::FDFile(const std::string& path)
{
	lastIoInfo = { 0 };

	handle = open(path.c_str(), O_BINARY | O_RDWR);
	ASSERT(handle != -1, path + " did not open!... errno was: " + std::to_string(errno) + ": " + strerror(errno));

	buffer = malloc(FILE_BUFFER_SIZE);
	ASSERT(buffer != NULL, "Could not allocate " + std::to_string(FILE_BUFFER_SIZE) + " bytes");
}

FDFile::~FDFile()
{
	close(handle);
	handle = -1;

	free(buffer);
	buffer = NULL;
}

#include <Windows.h>

bool FDFile::read(uint64_t offsetBytes, uint64_t numBytes)
{
	ASSERT((uint64_t)MAX_IO_SIZE > numBytes, "IO was requested to be larger than max allowed. Too large: " + std::to_string(numBytes));
	ASSERT(lseek64(handle, offsetBytes, SEEK_SET) == offsetBytes, "Could not seek to offset: " + std::to_string(offsetBytes));

	int numBytesRead;

	auto start = std::chrono::high_resolution_clock::now();

	// START FAST PATH
	numBytesRead = ::read(handle, buffer, static_cast<IO_SIZE_TYPE>(numBytes));
	// END FAST PATH

	auto end = std::chrono::high_resolution_clock::now();

	// Save last IO info
	lastIoInfo.numBytes = numBytes;
	lastIoInfo.offsetBytes = offsetBytes;
	lastIoInfo.elapsedMicroseconds = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

	//ioInfos.push_back(lastIoInfo);

	return numBytesRead == numBytes;
}

