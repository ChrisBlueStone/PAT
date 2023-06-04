cmake_minimum_required(VERSION 3.21.2)
project(PAT VERSION 1.0.0)

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
		CMAKE_CXX_FLAGS_RELEASE "-O2 -s"
		CMAKE_C_FLAGS_RELEASE "-O2 -s"
)

if (BUILD_EXAMPLES)
	add_subdirectory(${PROJECT_SOURCE_DIR}/examples)
endif()

if (BUILD_TESTING)
	add_subdirectory(${PROJECT_SOURCE_DIR}/tests)
endif()
