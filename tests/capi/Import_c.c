#include <assert.h>
#include <stdio.h>
#include <patimport.h>
#include "TestHelpers_c.h"

static const char file_contents[];
static PAT expected;

int Import_c(int argc, char **argv) {
	const char filename[] = "fileloadertest.pat";

	// Write PAT to file
	FILE *file = fopen(filename, "w");
	assert(NULL != file);
	assert(EOF != fputs(file_contents, file));
	assert(EOF != fclose(file));

	PAT *pat;
	assert(0 == pat_import(&pat, filename));

	// Remove file since we're done with it
	remove(filename);

	enum MatchResult result = VerifyMatch(pat, &expected);

	pat_free(pat);

	return result;
}

static const char file_contents[] =
	"rootA\n"
	"\tchild1\n"
	"\t\tgrandchild1-1\n"
	"        child2\n"
	"\n"
	"    child3\n"
	"\n"
	"        grandchild3-1\n"
	"    \tgrandchild3-2\n";

static PAT expected = {
	.whitespace = NULL,
	.token = NULL,
	.num_children = 1,
	.children = (PAT[]){
		{
			.whitespace = NULL,
			.token = "rootA",
			.num_children = 3,
			.children = (PAT[]){
				{
					.whitespace = "\t",
					.token = "child1",
					.num_children = 1,
					.children = (PAT[]){
						{
							.whitespace = "\t\t",
							.token = "grandchild1-1",
							.num_children = 0,
							.children = NULL
						}
					}
				},
				{
					.whitespace = "        ",
					.token = "child2",
					.num_children = 0,
					.children = NULL
				},
				{
					.whitespace = "    ",
					.token = "child3",
					.num_children = 2,
					.children = (PAT[]){
						{
							.whitespace = "        ",
							.token = "grandchild3-1",
							.children = NULL,
							.num_children = 0
						},
						{
							.whitespace = "    \t",
							.token = "grandchild3-2",
							.children = NULL,
							.num_children = 0
						}
					}
				}
			}
		}
	}
};
