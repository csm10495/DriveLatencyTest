# CMakeList.txt : CMake project for DriveLatencyTest, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

project ("DriveLatencyTest")

# Add source to this project's executable.
file(GLOB DST_SOURCES "*.cpp")
add_executable (DriveLatencyTest ${DST_SOURCES})

# Enable latest C++ Standard
set_property(TARGET DriveLatencyTest PROPERTY CXX_STANDARD 20)

# Constants
set(EXTERNAL_INCLUDE_FOLDER "${CMAKE_CURRENT_SOURCE_DIR}/local/")
set(CLI11_PATH "${EXTERNAL_INCLUDE_FOLDER}/CLI11.hpp")

# External Downloads
if (NOT EXISTS CLI11_PATH)
	file(DOWNLOAD "https://github.com/CLIUtils/CLI11/releases/download/v1.9.0/CLI11.hpp" ${CLI11_PATH})
endif()

# Preprocessor defines

# Full OS name
add_compile_definitions(DLT_OPERATING_SYSTEM=${CMAKE_SYSTEM_NAME})

# DLT_OS_WINDOWS/LINUX
string(TOUPPER DLT_OS_${CMAKE_SYSTEM_NAME} DLT_OS_NAME)
add_compile_definitions(${DLT_OS_NAME})

# For VC++ disable Windows-specific deprecations/warnings
add_compile_definitions("_CRT_SECURE_NO_WARNINGS")
add_compile_definitions("_CRT_NONSTDC_NO_DEPRECATE")
