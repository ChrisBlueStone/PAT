#include <PAT.h>
#include <PATImporter.h>
#include "TestHelpers.h"

#include <fstream>

int FileLoader(int argc, char** argv)
{
	const char* filename = "fileloadertest.pat";

	// Create PAT file
	std::ofstream out(filename);

	out << "rootA\n"
		"\tchild1\n"
		"\t\tgrandchild1-1\n"
		"        child2\n"
		"\n"
		"    child3\n"
		"\n"
		"        grandchild3-1\n"
		"    \tgrandchild3-2\n"
		"rootB\n";

	out.close();

	// Load PAT file
	PAT pat;
	PATImporter()(pat, filename);

	// Delete PAT file since we're done with it
	std::remove(filename);

	// Create expected PAT model to compare against.
	PAT expected = {
		"", "", {{
			"",
			"rootA", {{
				"\t",
				"child1", {{
					"\t\t",
					"grandchild1-1"
				}}
			}, {
				"        ",
				"child2", {}
			}, {
				"    ",
				"child3", {{
					"        ",
					"grandchild3-1", {}
				}, {
					"    \t",
					"grandchild3-2", {}
				}}
			}}
		}, {
			"",
			"rootB", {}
		}}
	};

	return VerifyMatch(pat, expected);
}
