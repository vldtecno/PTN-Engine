# This file is part of PTN Engine
#
# Copyright (c) 2019-2023 Eduardo Valgôde
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
	${pugixml_SOURCE_DIR}/include
	"./XML/src/"
	"./include"
)

if(CMAKE_COMPILER_IS_GNUCXX AND CMAKE_BUILD_TYPE STREQUAL "Coverage")
	SET(CMAKE_CXX_FLAGS "-g -O0 -fprofile-arcs -ftest-coverage")
	SET(CMAKE_C_FLAGS "-g -O0 -fprofile-arcs -ftest-coverage")
endif(CMAKE_COMPILER_IS_GNUCXX AND CMAKE_BUILD_TYPE STREQUAL "Coverage")

file( GLOB_RECURSE ImportExport_SRC
	"*.h"
	"*.cpp"
)

add_library (ImportExport ${ImportExport_SRC})
target_link_libraries(ImportExport PUBLIC
	pugixml
	PTN_Engine
)

set_target_properties(ImportExport PROPERTIES DEBUG_POSTFIX ${CMAKE_DEBUG_POSTFIX})
