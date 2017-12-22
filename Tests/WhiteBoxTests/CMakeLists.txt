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

add_library(google_test SHARED IMPORTED)

include_directories(
		${INCLUDE_DIR}
		${PROJECT_SOURCE_DIR}/Tests/WhiteBoxTests 
		${PROJECT_SOURCE_DIR}/PTN_Engine
		${GTEST_INCLUDE_DIRS}
	)

file( GLOB_RECURSE Test_SRC
		"*.h"
		"*.cpp"
	)
	
add_executable (WhiteBoxTestStatic ${Test_SRC})
if(MSVC)
	target_compile_definitions(WhiteBoxTestStatic PUBLIC GTEST_LINKED_AS_SHARED_LIBRARY)
endif(MSVC)

target_link_libraries(WhiteBoxTestStatic PUBLIC 
	${GTEST_BOTH_LIBRARIES}
	PTN_EngineStatic)	

add_executable (WhiteBoxTestDynamic ${Test_SRC})
target_link_libraries(WhiteBoxTestDynamic PUBLIC 
	${GTEST_BOTH_LIBRARIES}
	PTN_EngineDynamicExpAll)    
