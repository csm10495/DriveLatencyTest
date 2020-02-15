/*
	This file is part of DriveLatencytest (DLT). 
		DLT is released via an MIT License.
			Author(s): Charles Machalow

	This is the main entry point for DLT.
*/

#include "Assert.h"
#include "FDFile.h"

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

#include <chrono>
#include <thread>
#include <mutex>

std::mutex mtx;

void workload(std::string path)
{
	FDFile p(path);

	auto start = std::chrono::high_resolution_clock::now();

	int count = 0;
	uint64_t microsecondCount = 0;
	while (std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - start).count() < 1e9)
	{
		ASSERT_NO_MSG(p.read(0, 4096));
		microsecondCount += p.getLastIoInfo().elapsedMicroseconds;
		count += 1;
	}

	mtx.lock();
	std::cout << count << std::endl;
	std::cout << microsecondCount << std::endl;
	mtx.unlock();
}

int main(int argc, char** argv) {
	CLI::App app{ "Drive Latency Test (DLT) is a quick/dirty test to get an idea of drive latency in various conditions." };

	// flag arguments
	bool license = false;
	app.add_flag("--license", license, "If passed, present all license information.");

	// string arguments
	std::string path = "";
	app.add_option("-p,--path", path, "The path to the path we want to test with.");

	CLI11_PARSE(app, argc, argv);

	if (license)
	{
		printLicenseInfo();
	}

	// technically you can give --license without a path, though i don't know how to natively allow that.
	if (!path.size())
	{
		std::cerr << "No path was given. It is required to do a latency test." << std::endl;
		app.exit(CLI::RequiredError("--path/-p"));
	}

	std::list<std::thread> threads;
	for (size_t i = 0; i < 1; i++)
	{
		threads.push_back(std::thread(workload, path));
	}

	for (auto& i : threads)
	{
		i.join();
	}

	return EXIT_SUCCESS;
}