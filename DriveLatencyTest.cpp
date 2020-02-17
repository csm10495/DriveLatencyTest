/*
	This file is part of DriveLatencytest (DLT). 
		DLT is released via an MIT License.
			Author(s): Charles Machalow

	This is the main entry point for DLT.
*/

#include <thread>
#include <mutex>
#include "Assert.h"
#include "Config.h"
#include "FDFile.h"
#include "Workload.h"

#include "local/CLI11.hpp"

void printLicenseInfo()
{
	std::cout << "License Information:" << std::endl << std::endl;

	std::cout << R"(
		CLI11 1.8 Copyright(c) 2017 - 2019 University of Cincinnati, developed by Henry
		Schreiner under NSF AWARD 1414736. All rights reserved.

		Redistribution and use in source and binary forms of CLI11, with or without
		modification, are permitted provided that the following conditions are met :

	1. Redistributions of source code must retain the above copyright notice, this
		list of conditionsand the following disclaimer.
		2. Redistributions in binary form must reproduce the above copyright notice,
		this list of conditionsand the following disclaimer in the documentation
		and /or other materials provided with the distribution.
		3. Neither the name of the copyright holder nor the names of its contributors
		may be used to endorse or promote products derived from this software without
		specific prior written permission.

		THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
		ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
		WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
		DISCLAIMED.IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
		ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
		(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
	LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
		ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
		(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
		SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.)" << std::endl << std::endl;

	std::cout << "DriveLatencyTest (DLT) is licensed via the MIT License" << std::endl;

	std::cout << R"(
Copyright 2020 Charles Machalow

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.)" << std::endl << std::endl;
}

int main(int argc, char** argv) {
	CLI::App app{ "Drive Latency Test (DLT) is a quick/dirty test to get an idea of drive latency in various conditions." };

	constexpr uint16_t bitness = sizeof(void*) == 8 ? 64 : 86;
	std::cout << R"(
  ___  _  _____ 
 |   \| ||_   _|
 | |) | |__| |  
 |___/|____|_|  
                Drive Latency Test... Version 1.0.0 - )" << DLT_BUILD_TYPE << " x" << bitness << std::endl;

	// flag arguments
	bool license = false;

	app.add_flag("--license", license, "If passed, present all license information.");

	// string arguments
	std::string path = "";
	std::string workloadType = "SEQUENTIAL";

	app.add_option("-w,--workload_type", workloadType, "The type of workload to do. Possible values: SEQUENTIAL");
	app.add_option("-p,--path", path, "The path to the path we want to test with.");

	// numeric arguments
	uint32_t duration = 30;
	uint64_t ioSize = 512;
	uint64_t startingOffset = 0;
	uint64_t endingOffset = 2097152; // 1 GB in 512 byte offsets
	uint32_t numThreads = 1;
	
	app.add_option("-d,--duration", duration, "The duration for the test in seconds.");
	app.add_option("-i,--io_size", ioSize, "Size of each IO in bytes.");
	app.add_option("-s,--start_offset", startingOffset, "Starting offset in bytes.");
	app.add_option("-e,--end_offset", endingOffset, "Ending offset in bytes.");
	app.add_option("-t,--thread_count", numThreads, "Number of threads to run the workload.");

	CLI11_PARSE(app, argc, argv);

	if (license)
	{
		printLicenseInfo();
	}

	// technically you can give --license without a path, though i don't know how to natively allow that.
	if (!path.size())
	{
		std::cerr << "No path was given. It is required to do a latency test." << std::endl;
		exit(app.exit(CLI::RequiredError("--path/-p")));
	}

	CONFIG config
	{
		.Seconds = duration,
		.ThreadCount = numThreads,
		.IOSizeInBytes = ioSize,
		.StartingOffsetInBytes = startingOffset,
		.EndingOffsetInBytes = endingOffset,
		.WorkloadType = toWorkloadTypeEnum(workloadType),
		.Path = path
	};
	
	std::cout << "Configuration:" << std::endl << config.toString() << std::endl;

	std::list<std::thread> threads;
	std::list<WORKLOAD_RESULT> workloadResults;
	std::mutex mutex;

	for (uint32_t i = 0; i < numThreads; i++)
	{
		threads.push_back(std::thread([&config, &mutex, &workloadResults]() {

			Workload w(config);
			auto result = w.runWorkload();

			mutex.lock();
			workloadResults.push_back(result);
			//std::cout << "Result:" << std::endl << result.toString() << std::endl;
			mutex.unlock();
		}));
	}

	for (auto& thread : threads)
	{
		thread.join();
	}

	std::cout << "Final Result:" << std::endl << WORKLOAD_RESULT(workloadResults).toString() << std::endl;

	return EXIT_SUCCESS;
}