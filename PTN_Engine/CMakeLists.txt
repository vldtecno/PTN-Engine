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

#Two variants of the PTN_Engine library are created. 
include_directories (
		../
		${INCLUDE_DIR}
	)	
  
file( GLOB_RECURSE 
	PTN_Engine_SRC
		"*.h"
		"*.cpp"
	)

add_library (PTN_Engine 
	${PTN_Engine_SRC}
	)

target_compile_definitions(PTN_Engine PUBLIC
	_EXPORTING
   )
	
if(BUILD_TESTS AND MSVC)
	set_target_properties(PTN_Engine PROPERTIES WINDOWS_EXPORT_ALL_SYMBOLS 1)
endif(BUILD_TESTS AND MSVC)
	
target_include_directories (PTN_Engine PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
	 
set_target_properties(PTN_Engine PROPERTIES LINKER_LANGUAGE CXX)


