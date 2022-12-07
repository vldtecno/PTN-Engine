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

if(NOT BUILD_SHARED_LIBS)
	set_target_properties(WhiteBoxTest PROPERTIES SUFFIX ${EXECUTABLE_STATIC_POSTFIX}${CMAKE_EXECUTABLE_SUFFIX})
endif()	
set_target_properties(WhiteBoxTest PROPERTIES DEBUG_POSTFIX ${CMAKE_DEBUG_POSTFIX})

# Install rules
if(INSTALL_TESTS)
	install(TARGETS WhiteBoxTest
	RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
endif()  
