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
	   ${GTEST_INCLUDE_DIRS}
	)	

file( GLOB_RECURSE BlackBoxTest_SRC
		"*.h"
		"*.cpp"
	)
	
add_executable (BlackBoxTestDynamic ${BlackBoxTest_SRC})
target_link_libraries(BlackBoxTestDynamic PUBLIC 
	${GTEST_BOTH_LIBRARIES}
	PTN_EngineDynamic)
		
add_executable (BlackBoxTestStatic ${BlackBoxTest_SRC})
target_link_libraries(BlackBoxTestStatic PUBLIC 
	${GTEST_BOTH_LIBRARIES}
	PTN_EngineStatic)




