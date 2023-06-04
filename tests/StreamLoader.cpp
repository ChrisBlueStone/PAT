#include <PAT.h>
#include <PATImporter.h>

#include <sstream>
#include "TestHelpers.h"

MatchResult StreamLoader(int argc, char** argv)
{
	std::stringstream ss(
		"rootA\n"
		"\tchild1\n"
		"\t\tgrandchild1-1\n"
		"        child2\n"
		"\n"
		"    child3\n"
		"\n"
		"        grandchild3-1\n"
		"    \tgrandchild3-2\n"
		"rootB\n"
	);

	PAT pat;
	PATImporter()(pat, ss);

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
			"", "rootB", {}
		}}
	};

	return VerifyMatch(pat, expected);
}
