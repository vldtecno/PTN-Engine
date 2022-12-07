# This file is part of PTN Engine
# 
# Copyright (c) 2017 Eduardo Valgôde
# 
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
# 
# http://www.apache.org/licenses/LICENSE-2.0
# 
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

cmake_minimum_required (VERSION 3.8)

# The version number.
set (PTN_ENGINE_VERSION_MAJOR x)
set (PTN_ENGINE_VERSION_MINOR x)
set (PTN_ENGINE_VERSION_PATCH x)

project (PTN_Engine)

option(BUILD_SHARED_LIBS "Build shared libraries (DLLs)." OFF)

set(CMAKE_DEBUG_POSTFIX _d)
set(CMAKE_STATIC_LIBRARY_PREFIX lib)
set(EXECUTABLE_STATIC_POSTFIX _s)


set (CMAKE_CXX_STANDARD 17)
if(CMAKE_COMPILER_IS_GNUCXX)
	#set(MULTITHREADED_BUILD 8 CACHE STRING "How many threads are used to build the project")
	#set(CMAKE_MAKE_PROGRAM "${CMAKE_MAKE_PROGRAM} -j${MULTITHREADED_BUILD}")
	set(GCC_PTHREAD_COMPILE_FLAGS "-pthread")
	set(GCC_COVERAGE_COMPILE_FLAGS "-fprofile-arcs -ftest-coverage")
	set(GCC_COVERAGE_LINK_FLAGS "-lgcov")
	set(GCC_PTHREAD_LINK_FLAGS "-lpthread")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GCC_PTHREAD_COMPILE_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}")
	set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${GCC_PTHREAD_LINK_FLAGS} ${GCC_COVERAGE_LINK_FLAGS}")
	set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/scripts/cmake)
endif()

	if(MSVC)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4251 /MP")
endif(MSVC)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib/)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib/)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/)

set(INCLUDE_DIR ${PROJECT_SOURCE_DIR}/PTN_Engine/include)

option(BUILD_IMPORT_EXPORT "Builds importer and exporters")
option(BUILD_TESTS "Builds the unit tests" OFF)
option(BUILD_EXAMPLES "Builds the examples" OFF)


#Projects
add_subdirectory(PTN_Engine)

if(BUILD_TESTS)
	option(INSTALL_TESTS "Install tests" OFF)
	enable_testing()

	###
	# from https://crascit.com/2015/07/25/cmake-gtest/
	# Download and unpack googletest at configure time
	configure_file(cmake/gtest.CMakeLists.txt.in googletest-download/CMakeLists.txt)

	execute_process(COMMAND "${CMAKE_COMMAND}" -G "${CMAKE_GENERATOR}" .
		WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/googletest-download" )
	execute_process(COMMAND "${CMAKE_COMMAND}" --build .
		WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/googletest-download" )

	# Prevent GoogleTest from overriding our compiler/linker options
	# when building with Visual Studio
	set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

	# Add googletest directly to our build. This adds
	# the following targets: gtest, gtest_main, gmock
	# and gmock_main
	add_subdirectory("${CMAKE_BINARY_DIR}/googletest-src"
		"${CMAKE_BINARY_DIR}/googletest-build")

	# The gtest/gmock targets carry header search path
	# dependencies automatically when using CMake 2.8.11 or
	# later. Otherwise we have to add them here ourselves.
	if(CMAKE_VERSION VERSION_LESS 2.8.11)
		include_directories("${gtest_SOURCE_DIR}/include"
			"${gmock_SOURCE_DIR}/include")
	endif()
	###

	add_subdirectory(Tests/WhiteBoxTests)
	add_subdirectory(Tests/BlackBoxTests)
endif(BUILD_TESTS)

if(BUILD_EXAMPLES)
	option(INSTALL_EXAMPLES "Install examples" OFF)
	add_subdirectory(Examples)
endif(BUILD_EXAMPLES)

option(INSTALL_PTN_ENGINE "Enable installation of PTN Engine. (Projects embedding PTN Engine may want to turn this OFF.)" ON )

include(CMakeDependentOption)
include(GNUInstallDirs)
