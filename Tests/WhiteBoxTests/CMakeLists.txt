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

find_path(GOOGLE_TEST_INCLUDE_DIR google_test)
find_path(GOOGLE_TEST_BUILD_DIR google_test_build)

add_library(google_test SHARED IMPORTED)

enable_testing()
set(PROJECT_TEST_NAME &{PROJECT_NAME_STR}_test)

include_directories(
		${INCLUDE_DIR}
		${PROJECT_SOURCE_DIR}/Tests/WhiteBoxTests 
		${PROJECT_SOURCE_DIR}/PTN_Engine
		${GOOGLE_TEST_INCLUDE_DIR}
	)

file( GLOB_RECURSE Test_SRC
		"*.h"
		"*.cpp"
	)

if(CMAKE_COMPILER_IS_GNUCXX)
	message(STATUS "GCC detected")	
	link_directories(${GOOGLE_TEST_BUILD_DIR}/googlemock/gtest)
endif(CMAKE_COMPILER_IS_GNUCXX)

if(MSVC)
	message(STATUS "MSVC detected")
	link_directories(${GOOGLE_TEST_BUILD_DIR}/googletest)
	link_directories(${GOOGLE_TEST_BUILD_DIR}/googlemock)
	link_directories(${GOOGLE_TEST_BUILD_DIR}/googlemock/gtest)
endif(MSVC)
	
add_executable (WhiteBoxTestStatic ${Test_SRC})
if(MSVC)
	target_compile_definitions(WhiteBoxTestStatic PUBLIC GTEST_LINKED_AS_SHARED_LIBRARY)
endif(MSVC)
target_link_libraries(WhiteBoxTestStatic PUBLIC gtest gtest_main)
target_link_libraries(WhiteBoxTestStatic PUBLIC PTN_EngineStatic)
	

add_executable (WhiteBoxTestDynamic ${Test_SRC})
target_link_libraries(WhiteBoxTestDynamic PUBLIC gtest gtest_main)
target_link_libraries(WhiteBoxTestDynamic PUBLIC PTN_EngineDynamicExpAll)    
