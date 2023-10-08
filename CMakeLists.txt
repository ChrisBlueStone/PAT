cmake_minimum_required(VERSION 3.21.2)
project(PAT VERSION 1.0.0)
project(PAT_C VERSION 1.0.0 LANGUAGES C)

include(CTest)
enable_testing()

option(BUILD_EXAMPLES "Build example projects" ON)

add_library(PAT
	${PROJECT_SOURCE_DIR}/src/cppapi/PAT.cpp
	${PROJECT_SOURCE_DIR}/src/cppapi/PATImporter.cpp
	${PROJECT_SOURCE_DIR}/src/cppapi/PATExporter.cpp
	${PROJECT_SOURCE_DIR}/src/cppapi/PATConverters.cpp
)

target_include_directories(PAT
	PUBLIC
		${PROJECT_SOURCE_DIR}/src/cppapi
)

set_target_properties(PAT
	PROPERTIES
		CXX_STANDARD 11
		CXX_STANDARD_REQUIRED True
		CMAKE_CXX_FLAGS_RELEASE "-O2 -s -Wall -Wextra -pendantic"
)

add_library(PAT_C
	${PROJECT_SOURCE_DIR}/src/capi/pat.c
	${PROJECT_SOURCE_DIR}/src/capi/patimport.c
)

target_include_directories(PAT_C
	PUBLIC
		${PROJECT_SOURCE_DIR}/src/capi
)

set_target_properties(PAT_C
	PROPERTIES
		C_STANDARD 99
		C_STANDARD_REQUIRED True
		CMAKE_C_FLAGS_RELEASE "-O2 -s -Wall -Wextra -pendantic"
)

if (BUILD_EXAMPLES)
	add_subdirectory(${PROJECT_SOURCE_DIR}/examples)
endif()

if (BUILD_TESTING)
	add_subdirectory(${PROJECT_SOURCE_DIR}/tests)
endif()
