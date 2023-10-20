# This file is part of PTN Engine
# 
# Copyright (c) 2017-2023 Eduardo Valgôde
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

include_directories(
		${INCLUDE_DIR}
		${PROJECT_SOURCE_DIR}/Tests/BlackBoxTests
		${gtest_SOURCE_DIR}/include
	)

file( GLOB_RECURSE BlackBoxTest_SRC
		"*.h"
		"*.cpp"
	)

add_executable (BlackBoxTest ${BlackBoxTest_SRC})
target_link_libraries(BlackBoxTest PUBLIC 
	gtest
	gtest_main
	gmock
	gmock_main
	PTN_Engine)

if(BUILD_SHARED_LIBS)
	set(BlackBoxTestsExecutable "BlackBoxTest${CMAKE_EXECUTABLE_SUFFIX}")
else()
	set(BlackBoxTestsExecutable "BlackBoxTest${EXECUTABLE_STATIC_POSTFIX}${CMAKE_EXECUTABLE_SUFFIX}")
endif()

add_test(NAME RoundRobin_1
		COMMAND ${BlackBoxTestsExecutable} --gtest_filter=*RoundRobin_1)
add_test(NAME RoundRobin_1_detached
		COMMAND ${BlackBoxTestsExecutable} --gtest_filter=*RoundRobin_1_detached)
add_test(NAME RoundRobin_1_continuous2
		COMMAND ${BlackBoxTestsExecutable} --gtest_filter=*RoundRobin_1_continuous2)
add_test(NAME FreeChoice_1
		COMMAND ${BlackBoxTestsExecutable} --gtest_filter=*FreeChoice_1)
add_test(NAME FreeChoice_1_detached
		COMMAND ${BlackBoxTestsExecutable} --gtest_filter=*FreeChoice_1_detached)
add_test(NAME Weights_1
		COMMAND ${BlackBoxTestsExecutable} --gtest_filter=*Weights_1)
add_test(NAME Weights_1_detached
		COMMAND ${BlackBoxTestsExecutable} --gtest_filter=*Weights_1_detached)
add_test(NAME Inhibited_SingleThread
		COMMAND ${BlackBoxTestsExecutable} --gtest_filter=*Inhibited_SingleThread)
add_test(NAME Inhibited_1
		COMMAND ${BlackBoxTestsExecutable} --gtest_filter=*Inhibited_1)
add_test(NAME Inhibited_1_detached
		COMMAND ${BlackBoxTestsExecutable} --gtest_filter=*Inhibited_1_detached)
add_test(NAME ThreadSafety
		COMMAND ${BlackBoxTestsExecutable} --gtest_filter=*ThreadSafety)
add_test(NAME RegisteredMethods
		COMMAND ${BlackBoxTestsExecutable} --gtest_filter=*RegisteredMethods)
add_test(NAME AlreadyRegisteredMethods
		COMMAND ${BlackBoxTestsExecutable} --gtest_filter=*AlreadyRegisteredMethods)
add_test(NAME AdditionalActivationConditions
		COMMAND ${BlackBoxTestsExecutable} --gtest_filter=*AdditionalActivationConditions)
add_test(NAME Ordering
		COMMAND ${BlackBoxTestsExecutable} --gtest_filter=*Ordering)
add_test(NAME ThreadSafety_1
		COMMAND ${BlackBoxTestsExecutable} --gtest_filter=*ThreadSafety_1)

set_target_properties(BlackBoxTest PROPERTIES DEBUG_POSTFIX ${CMAKE_DEBUG_POSTFIX})

# Install rules
if(INSTALL_TESTS)
	install(TARGETS BlackBoxTest
	RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
endif()
