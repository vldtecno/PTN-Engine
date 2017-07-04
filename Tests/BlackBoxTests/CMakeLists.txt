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

enable_testing()
set(PROJECT_TEST_NAME &{PROJECT_NAME_STR}_test)

include_directories(
	   ${INCLUDE_DIR}
	   ${PROJECT_SOURCE_DIR}/Tests/BlackBoxTests 
	   ${GOOGLE_TEST_INCLUDE_DIR}
	)	

file( GLOB_RECURSE BlackBoxTest_SRC
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
	
add_executable (BlackBoxTestDynamic ${BlackBoxTest_SRC})
target_link_libraries(BlackBoxTestDynamic PUBLIC gtest gtest_main)
target_link_libraries(BlackBoxTestDynamic PUBLIC PTN_EngineDynamic)
		
add_executable (BlackBoxTestStatic ${BlackBoxTest_SRC})
target_link_libraries(BlackBoxTestStatic PUBLIC gtest gtest_main)
target_link_libraries(BlackBoxTestStatic PUBLIC PTN_EngineStatic)




