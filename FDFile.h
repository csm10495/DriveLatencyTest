/*
	This file is part of DriveLatencytest (DLT).
		DLT is released via an MIT License.
			Author(s): Charles Machalow

	This is the header for the FDFile class
*/

#pragma once

#include <list>
#include <string>

#include "Config.h"
#include "Stats.h"

//! File-descriptor based file class
class FDFile
{
public:

	/*! Constructor that takes in the current config
	*/
	FDFile(const CONFIG &config);

	//! Destructor. Will close the file.
	~FDFile();

	/*! Does a read for the given numBytes at the given offset. Returns true on success, false on failure.
			Will place read data in the buffer on success
	*/
	bool read(uint64_t offsetBytes, uint64_t numBytes);

	//! gets the previous IO information
	const inline IO_INFO& getLastIoInfo() const
	{
		return lastIoInfo;
	}

	//! The buffer we read into. Should be sufficiently large enough for any IO
	void* buffer;
private:

	//! the handle to the drive
	int handle;

	//! info on the previous IO
	IO_INFO lastIoInfo;

	// disable some things
	FDFile& operator=(const FDFile&) = delete;
	FDFile(const FDFile&) = delete;
	FDFile() = default;
};