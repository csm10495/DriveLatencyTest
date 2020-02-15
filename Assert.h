/*
	This file is part of DriveLatencytest (DLT).
		DLT is released via an MIT License.
			Author(s): Charles Machalow

	This is the header for assert-related functionality for when things go really wrong.
*/

#include <cassert>
#include <cstdlib>
#include <iostream>
#include <string>

#ifndef NDEBUG

namespace details
{
	//! Internal assert function, will assert if expressionValue calls for it.
	void _assert(int expressionValue, const std::string& expression, const std::string& file, const std::string& function, const size_t lineNumber);
}

#define ASSERT(expression) \
	details::_assert(expression, #expression, __FILE__, __func__, __LINE__)

#else // ^NDEBUG not defined
#define ASSERT(expression, file, function, lineNumber)
#endif // ^NDEBUG defined

