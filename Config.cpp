/*
	This file is part of DriveLatencytest (DLT).
		DLT is released via an MIT License.
			Author(s): Charles Machalow

	This is the implementation for config keeping things
*/

#include "Config.h"

std::string _CONFIG::toString() const
{
	std::string retStr = "";

	retStr += "Duration (Seconds)                 : " + std::to_string(Seconds) + "\n";
	retStr += "IO Size (Bytes)                    : " + std::to_string(IOSizeInBytes) + "\n";
	retStr += "Starting Offset (Bytes)            : " + std::to_string(StartingOffsetInBytes) + "\n";
	retStr += "Ending Offset (Bytes)              : " + std::to_string(EndingOffsetInBytes) + "\n";
	retStr += "Workload Type                      : " + ::toString(WorkloadType) + "\n";
	retStr += "Path                               : " + Path + "\n";

	return retStr;
}