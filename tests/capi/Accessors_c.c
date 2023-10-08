#include "pat.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

static const PAT *pat;

int Accessors_c(int argc, char **argv) {
	// Last child's token should be "B"
	assert(strcmp(pat_get_value(pat), "B") == 0);
	assert(strcmp(pat_get_value(pat), "A") != 0);

	const char *value;
	// Index of the child retrieved should be 2 and the token should be "B".
	assert(pat_try_get_value(pat, &value) == 2);
	assert(strcmp(value, "B") == 0);

	// First child should not have any children
	assert(pat_try_get_value(&pat->children[0], &value) == -1);
	assert(strcmp(value, "B") == 0); // Value should be retained

	// Test indexing
	const PAT *A = &pat->children[0];
	assert(strcmp(A->token, "A") == 0);
	assert(A->num_children == 0);

	// Test finding items
	assert(pat_first(pat, "B") == &pat->children[2]);
	assert(pat_last(pat, "A") == &pat->children[1]);

	// Test drilling down to last "cc"
	assert(pat_last(pat_first(pat_last(pat, "A"), "c"), "cc") == &pat->children[1].children[2].children[1]);

	// Test drilling down to invalid nodes
	assert(pat_first(pat_first(pat, "Z"), "Z") == NULL);
	assert(pat_first(pat_first(pat, "A"), "Z") == NULL);
	assert(pat_last(pat_last(pat, "Z"), "Z") == NULL);
	assert(pat_last(pat_last(pat, "A"), "Z") == NULL);

	return 0;
}

static const PAT *pat = &(PAT){
	"", "", 3, (PAT[]){
		{
			"",  "A", 0, NULL
		}, {
			"", "A", 3, (PAT[]){
				"", "a", 1, (PAT[]){
					"", "aa", 0, NULL
				}, {
					"", "b", 0, NULL
				}, {
					"", "c", 2, (PAT[]){
						{
							"", "cc", 0, NULL
						}, {
							"", "cc", 0, NULL
						}
					}
				}
			}
		}, {
			"", "B", 0, NULL
		}
	}
};
