/*
	This file is part of DriveLatencytest (DLT).
		DLT is released via an MIT License.
			Author(s): Charles Machalow

	This is the implementation for assert-related functionality for when things go really wrong.
*/

#ifndef NDEBUG

#include "Assert.h"
#include <stdlib.h>

#ifndef DLT_OS_WINDOWS
#include <libgen.h>
#else 
// Give Windows an implementation for basename()
std::string basename(const std::string& path)
{
	char dir[4096] = { 0 };
	char ext[4096] = { 0 };
	_splitpath_s(path.c_str(),
		NULL, 0,
		NULL, 0,
		dir, sizeof(dir),
		ext, sizeof(ext));

	return std::string(dir) + ext;
}
#endif

namespace details
{
	void _assert(int expressionValue, const std::string& expression, const std::string& file, const std::string& function, const size_t lineNumber, const std::string& message)
	{
		if (!expressionValue)
			[[unlikely]]
		{
			
			std::string fileName = basename((char*)file.c_str());
			std::cerr << "ASSERT: (" << fileName << ":" << function << ":" << lineNumber << "): " << "EXPR: " << expression << ". " << message << std::endl;
			abort();
		}
	}
}

#endif // #ifndef NDEBUG