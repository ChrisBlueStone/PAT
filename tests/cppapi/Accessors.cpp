#include <PAT.h>

#include <assert.h>
#include <string>

int Accessors(int argc, char** argv)
{
	PAT pat = {
		"", "", {{
			"", "A", {}
		}, {
			"", "A", {{
				"", "a", {{
					"", "aa"
				}}
			}, {
				"", "b", {}
			}, {
				"", "c", {{
					"", "cc", {}
				}, {
					"", "cc", {}
				}}
			}}
		}, {
			"", "B", {}
		}}
	};

	// Test GetValue
	std::string value;
	assert(pat.GetValue() == "B");
	assert(pat.GetValue("default") == "B");
	assert(pat.TryGetValue(value) == true);
	assert(value == "B");
	assert(pat[0].TryGetValue(value) == false);
	assert(value == "B"); // Value should be retained.

	// Test indexing
	const PAT& A = pat[0];
	assert(A.token == "A");
	assert(A.children.size() == 0);

	const PAT& A2 = pat["A"];
	assert(A2.token == "A");
	assert(A2.children.size() == 3);

	// Test more indexing
	assert(pat[1].token == "A");
	assert(pat[2].token == "B");
	assert(pat["A"][1].token == "b");
	
	// Test finding and selecting
	assert(&pat.Find("A") == &A);
	assert(&pat.Find("A", A2) == &A2);
	assert(&pat.Find("null") != &pat[0]);
	assert(pat.Find("null").token != "null");
	assert(&pat.Select("A", 0) == &A);
	assert(&pat.Select("A", 1) == &A2);
	
	return 0;
}
