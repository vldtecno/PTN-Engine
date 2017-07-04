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
set (PTN_ENGINE_VERSION_MAJOR 1)
set (PTN_ENGINE_VERSION_MINOR 0)
set (PTN_ENGINE_VERSION_PATCH 0)

project (PTN_Engine)


if(CMAKE_COMPILER_IS_GNUCXX)
	#set(MULTITHREADED_BUILD 8 CACHE STRING "How many threads are used to build the project")
	#set(CMAKE_MAKE_PROGRAM "${CMAKE_MAKE_PROGRAM} -j${MULTITHREADED_BUILD}")
	set(GCC_COVERAGE_COMPILE_FLAGS "-fprofile-arcs -ftest-coverage -std=c++0x")
	set(GCC_COVERAGE_LINK_FLAGS "-lgcov -lpthread")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}")
	set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${GCC_COVERAGE_LINK_FLAGS}")	
endif()

if(MSVC)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4251 /MP")
endif(MSVC)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib/)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib/)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/)

set(INCLUDE_DIR ${PROJECT_SOURCE_DIR}/PTN_Engine/include)

option(BUILD_TESTS "Builds the unit tests" OFF)
option(BUILD_SAMPLES "Builds the samples" OFF)

#Projects
add_subdirectory(PTN_Engine)

if(BUILD_TESTS)
add_subdirectory(Tests/WhiteBoxTests)
add_subdirectory(Tests/BlackBoxTests)
endif(BUILD_TESTS)

if(BUILD_SAMPLES)
add_subdirectory(Samples)
endif(BUILD_SAMPLES)

