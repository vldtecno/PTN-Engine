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

#Two variants of the PTN_Engine library are created. 
include_directories (
	../
	${INCLUDE_DIR})

file (GLOB
	PTN_Engine_SRC_1
	"*.h"
	"*.cpp")
file (GLOB_RECURSE
	PTN_Engine_SRC_2
	"import/*.h"
	"import/*.cpp")

file (GLOB_RECURSE
	PTN_Engine_SRC_4
	"JobQueue/*.h"
	"JobQueue/*.cpp")

file (GLOB_RECURSE
	PTN_Engine_SRC_5
	"Utilities/*.h"
	"Utilities/*.cpp")

file (GLOB
	PTN_Engine_SRC
	${PTN_Engine_SRC_1}
	${PTN_Engine_SRC_2}
	${PTN_Engine_SRC_3}
	${PTN_Engine_SRC_4}
	${PTN_Engine_SRC_5})

add_library (PTN_Engine 
	${PTN_Engine_SRC})

target_compile_definitions (PTN_Engine PUBLIC
	_EXPORTING)

if (CMAKE_COMPILER_IS_GNUCXX AND CMAKE_BUILD_TYPE STREQUAL "Coverage")
	SET (CMAKE_CXX_FLAGS "-g -O0 -fprofile-arcs -ftest-coverage")
	SET (CMAKE_C_FLAGS "-g -O0 -fprofile-arcs -ftest-coverage")
endif (CMAKE_COMPILER_IS_GNUCXX AND CMAKE_BUILD_TYPE STREQUAL "Coverage")

if (BUILD_TESTS AND MSVC)
	set_target_properties (PTN_Engine PROPERTIES WINDOWS_EXPORT_ALL_SYMBOLS 1)
endif (BUILD_TESTS AND MSVC)

target_include_directories (PTN_Engine PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

set_target_properties (PTN_Engine PROPERTIES LINKER_LANGUAGE CXX)

########################################################################
#
# Install rules
if (INSTALL_PTN_ENGINE)
	install(TARGETS PTN_Engine
		RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
		ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
		LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

	install(DIRECTORY ${CMAKE_SOURCE_DIR}/PTN_Engine/include/
		DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

	# configure and install pkgconfig files
	configure_file(
		cmake/PTN_Engine.pc.in
		"${CMAKE_BINARY_DIR}/PTN_Engine.pc"
		@ONLY)

	install(FILES "${CMAKE_BINARY_DIR}/PTN_Engine.pc"
		DESTINATION "${CMAKE_INSTALL_LIBDIR}/pkgconfig")
endif ()

if(BUILD_IMPORT_EXPORT)
	### pugixml

	configure_file(cmake/pugixml.CMakeLists.txt.in pugixml-download/CMakeLists.txt)

	execute_process(COMMAND "${CMAKE_COMMAND}" -G "${CMAKE_GENERATOR}" .
		WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/pugixml-download" )
	execute_process(COMMAND "${CMAKE_COMMAND}" --build .
		WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/pugixml-download" )

	#define PUGIXML_WCHAR_MODE
#	add_definitions(-DPUGIXML_WCHAR_MODE)

#	set(pugixml_force_shared_crt ON CACHE BOOL "" FORCE)

	add_subdirectory("${CMAKE_BINARY_DIR}/pugixml-src"
		"${CMAKE_BINARY_DIR}/pugixml-build")

	include_directories("${pugixml_SOURCE_DIR}/include"
		"${pugixml_SOURCE_DIR}/include")

	######
	add_subdirectory(ImportExport)
endif(BUILD_IMPORT_EXPORT)
