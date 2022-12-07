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

set(PROJECT_TEST_NAME PhoneMenuXML)

include_directories(
	   ${INCLUDE_DIR}
	   ${PROJECT_SOURCE_DIR}/PTN_Engine/ImportExport/include
	   ${PROJECT_SOURCE_DIR}/Examples/PhoneMenuXML
	)	

file( GLOB_RECURSE PhoneMenuXML_SRC
		"*.h"
		"*.cpp"
	)

add_executable (PhoneMenuXML ${PhoneMenuXML_SRC})
target_link_libraries(PhoneMenuXML PUBLIC 
	PTN_Engine 
	ImportExport)

if(NOT BUILD_SHARED_LIBS)
	set_target_properties(PhoneMenuXML PROPERTIES SUFFIX ${EXECUTABLE_STATIC_POSTFIX}${CMAKE_EXECUTABLE_SUFFIX})
endif()
set_target_properties(PhoneMenuXML PROPERTIES DEBUG_POSTFIX ${CMAKE_DEBUG_POSTFIX})

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/assets/PhoneMenu.xml DESTINATION ${CMAKE_BINARY_DIR}/bin)


# Install rules
if(INSTALL_EXAMPLES)
  install(TARGETS PhoneMenuXML
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
endif()



