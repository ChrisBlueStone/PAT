create_test_sourcelist (Tests
  Tests.cxx
  TestHelpers.cpp
  StreamLoader.cpp
  FileLoader.cpp
  PATAccessors.cpp
  Converters.cpp
)

# add the executable
add_executable (Tests
	${Tests}
	TestHelpers.cpp
)

target_link_libraries(Tests
	PRIVATE
		PAT
)

# add_custom_command(TARGET Tests
# 	PRE_BUILD
# 	COMMAND ${CMAKE_COMMAND} -E copy_file ${PROJECT_SOURCE_DIR}/tests/test.pat ${PROJECT_BINARY_DIR}/test.pat)

# remove the test driver source file
set (TestsToRun ${Tests})
remove (TestsToRun Tests.cxx)

# Add all the ADD_TEST for each test
foreach (test ${TestsToRun})
  get_filename_component (TName ${test} NAME_WE)
  add_test (NAME ${TName} COMMAND Tests ${TName})
endforeach ()
