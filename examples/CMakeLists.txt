cmake_minimum_required(VERSION 3.21.2)
project (PATExamples)

option(BUILD_PATNORMALIZER "Builds the PAT Normalizer app" ON)

if (BUILD_PATNORMALIZER)
	add_subdirectory(${PROJECT_SOURCE_DIR}/PATNormalizer)
endif()
