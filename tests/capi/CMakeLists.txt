create_test_sourcelist(CAPITests
  CAPITests.c
  CreateFree.c
  TestHelpers_c.c
  Import_c.c
  Accessors_c.c
  TreeModification_c.c
)

# add the executable
add_executable(CAPITests
	${CAPITests}
)

target_link_libraries(CAPITests
	PRIVATE
		PAT_C
)

# remove the test driver source file
set (TestsToRun ${CAPITests})
remove (TestsToRun CAPITests.c)

# Add all the ADD_TEST for each test
foreach (test ${TestsToRun})
  get_filename_component (TName ${test} NAME_WE)
  add_test (NAME ${TName} COMMAND CAPITests ${TName})
endforeach ()
