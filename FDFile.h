/*
	This file is part of DriveLatencytest (DLT).
		DLT is released via an MIT License.
			Author(s): Charles Machalow

	This is the header for the FDFile class
*/

#include <list>
#include <string>

#include "Stats.h"

//! File-descriptor based file class
class FDFile
{
public:

	/*! Constructor that takes the path to open. 
	*/
	FDFile(const std::string& path);

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

	std::list<IO_INFO> ioInfos;
};