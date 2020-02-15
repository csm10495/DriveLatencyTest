/*
	This file is part of DriveLatencytest (DLT). 
		DLT is released via an MIT License.
			Author(s): Charles Machalow

	This is the main entry point for DLT.
*/

#include "Assert.h"

#include "local/CLI11.hpp"

int main(int argc, char** argv) {
	CLI::App app{ "Drive Latency Test (DLT) is a quick/dirty test to get an idea of drive latency in various conditions." };

	std::string path;
	app.add_option("-p,--path", path, "The path to the path we want to test with.")->required();

	CLI11_PARSE(app, argc, argv);

	return EXIT_SUCCESS;
}