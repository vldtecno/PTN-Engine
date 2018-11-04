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

if(NOT BUILD_SHARED_LIBS)
	set_target_properties(BlackBoxTest PROPERTIES SUFFIX ${EXECUTABLE_STATIC_POSTFIX}${CMAKE_EXECUTABLE_SUFFIX})
endif()
set_target_properties(BlackBoxTest PROPERTIES DEBUG_POSTFIX ${CMAKE_DEBUG_POSTFIX})

# Install rules
if(INSTALL_TESTS)
	install(TARGETS BlackBoxTest
	RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
endif()
