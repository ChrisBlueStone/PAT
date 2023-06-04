cmake_minimum_required(VERSION 3.21.2)
project(PATNormalizer VERSION 1.0.0)

include(CTest)
enable_testing()

add_executable(PATNormalizer
	${PROJECT_SOURCE_DIR}/patnormalizer.cpp
)

target_link_libraries(PATNormalizer
	PRIVATE
		PAT
)

set_target_properties(PATNormalizer
	PROPERTIES
		CXX_STANDARD 11
		CXX_STANDARD_REQUIRED True
		CMAKE_CXX_FLAGS_RELEASE "-O2 -s"
		CMAKE_C_FLAGS_RELEASE "-O2 -s"
)
