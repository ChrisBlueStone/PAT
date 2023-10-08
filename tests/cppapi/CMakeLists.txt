create_test_sourcelist(Tests
  Tests.cxx
  TestHelpers.cpp
  StreamLoader.cpp
  FileLoader.cpp
  Accessors.cpp
  Converters.cpp
)

# add the executable
add_executable(Tests
	${Tests}
)

target_link_libraries(Tests
	PRIVATE
		PAT
)

# remove the test driver source file
set (TestsToRun ${Tests})
remove (TestsToRun Tests.cxx)

# Add all the ADD_TEST for each test
foreach (test ${TestsToRun})
  get_filename_component (TName ${test} NAME_WE)
  add_test (NAME ${TName} COMMAND Tests ${TName})
endforeach ()
