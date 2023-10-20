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
		${PROJECT_SOURCE_DIR}/Tests/WhiteBoxTests 
		${PROJECT_SOURCE_DIR}
		${gtest_SOURCE_DIR}/include
	)

file( GLOB_RECURSE Test_SRC
		"*.h"
		"*.cpp"
	)

add_executable (WhiteBoxTest ${Test_SRC})
if(NOT BUILD_SHARED_LIBS AND MSVC)
	target_compile_definitions(WhiteBoxTest PUBLIC GTEST_LINKED_AS_SHARED_LIBRARY)
endif(NOT BUILD_SHARED_LIBS AND MSVC)

target_link_libraries(WhiteBoxTest PUBLIC 
	gtest 
	gtest_main
	gmock
	gmock_main
	PTN_Engine)	

if(BUILD_SHARED_LIBS)
	set(WhiteBoxTestsExecutable "WhiteBoxTest${CMAKE_EXECUTABLE_SUFFIX}")
else()
	set(WhiteBoxTestsExecutable "WhiteBoxTest${EXECUTABLE_STATIC_POSTFIX}${CMAKE_EXECUTABLE_SUFFIX}")
endif()

add_test(NAME InputPlace
		COMMAND ${WhiteBoxTestsExecutable} --gtest_filter=*InputPlace)
add_test(NAME EnterPlaceDefault
		COMMAND ${WhiteBoxTestsExecutable} --gtest_filter=*EnterPlaceDefault)
add_test(NAME EnterPlace
		COMMAND ${WhiteBoxTestsExecutable} --gtest_filter=*EnterPlace)
add_test(NAME ExitPlace
		COMMAND ${WhiteBoxTestsExecutable} --gtest_filter=*ExitPlace)
add_test(NAME Overflow
		COMMAND ${WhiteBoxTestsExecutable} --gtest_filter=*Overflow)
add_test(NAME NullTokens
		COMMAND ${WhiteBoxTestsExecutable} --gtest_filter=*NullTokens)
add_test(NAME NotEnoughTokens
		COMMAND ${WhiteBoxTestsExecutable} --gtest_filter=*NotEnoughTokens)
add_test(NAME T1
		COMMAND ${WhiteBoxTestsExecutable} --gtest_filter=*T1)
add_test(NAME T2
		COMMAND ${WhiteBoxTestsExecutable} --gtest_filter=*T2)
add_test(NAME T3
		COMMAND ${WhiteBoxTestsExecutable} --gtest_filter=*T3)
add_test(NAME T_Weights
		COMMAND ${WhiteBoxTestsExecutable} --gtest_filter=*T_Weights)
add_test(NAME T_ZeroValueWeightException
		COMMAND ${WhiteBoxTestsExecutable} --gtest_filter=*T_ZeroValueWeightException)
add_test(NAME T_ActivationWeightDimensionException
		COMMAND ${WhiteBoxTestsExecutable} --gtest_filter=*T_ActivationWeightDimensionException)
add_test(NAME T_DestinationWeightDimensionException
		COMMAND ${WhiteBoxTestsExecutable} --gtest_filter=*T_DestinationWeightDimensionException)
add_test(NAME T_ActivationPlaceRepetitionException
		COMMAND ${WhiteBoxTestsExecutable} --gtest_filter=*T_ActivationPlaceRepetitionException)
add_test(NAME T_DestinationPlaceRepetitionException
		COMMAND ${WhiteBoxTestsExecutable} --gtest_filter=*T_DestinationPlaceRepetitionException)

set_target_properties(WhiteBoxTest PROPERTIES DEBUG_POSTFIX ${CMAKE_DEBUG_POSTFIX})

# Install rules
if(INSTALL_TESTS)
	install(TARGETS WhiteBoxTest
	RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
endif()  
